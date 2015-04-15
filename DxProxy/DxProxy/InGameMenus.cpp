/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <InGameMenu.cpp> and
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
#include "D3D9ProxySurface.h"
#include "StereoViewFactory.h"
#include "MotionTrackerFactory.h"
#include "HMDisplayInfoFactory.h"
#include <typeinfo>
#include <assert.h>
#include <comdef.h>
#include <tchar.h>
#include "Resource.h"
#include <D3DX9Shader.h>

#ifdef _DEBUG
#include "DxErr.h"
#endif

#include "Version.h"

#define MENU_ITEM_SEPARATION  40

using namespace VRBoost;

bool D3DProxyDevice::InitVPMENU()
{
	#ifdef SHOW_CALLS
		OutputDebugString("called InitVPMENU");
	#endif
	hudFont = NULL;
	menuTime = (float)GetTickCount()/1000.0f;
	ZeroMemory(&m_configBackup, sizeof(m_configBackup));
	screenshot = (int)false;
	m_bForceMouseEmulation = false;
	m_bVRBoostToggle = true;
	m_bPosTrackingToggle = true;
	m_showVRMouse = 0;
	m_fVRBoostIndicator = 0.0f;
	VPMENU_mode = VPMENU_Modes::INACTIVE;
	borderTopHeight = 0.0f;
	menuTopHeight = 0.0f;
	menuVelocity = D3DXVECTOR2(0.0f, 0.0f);
	menuAttraction = D3DXVECTOR2(0.0f, 0.0f);
	hud3DDepthMode = HUD_3D_Depth_Modes::HUD_DEFAULT;
	gui3DDepthMode = GUI_3D_Depth_Modes::GUI_DEFAULT;
	oldHudMode = HUD_3D_Depth_Modes::HUD_DEFAULT;
	oldGuiMode = GUI_3D_Depth_Modes::GUI_DEFAULT;
	hud3DDepthPresets[0] = 0.0f;
	hud3DDepthPresets[1] = 0.0f;
	hud3DDepthPresets[2] = 0.0f;
	hud3DDepthPresets[3] = 0.0f;
	hudDistancePresets[0] = 0.5f;
	hudDistancePresets[1] = 0.9f;
	hudDistancePresets[2] = 0.3f;
	hudDistancePresets[3] = 0.0f;
	gui3DDepthPresets[0] = 0.0f;
	gui3DDepthPresets[1] = 0.0f;
	gui3DDepthPresets[2] = 0.0f;
	gui3DDepthPresets[3] = 0.0f;
	guiSquishPresets[0] = 0.6f;
	guiSquishPresets[1] = 0.5f;
	guiSquishPresets[2] = 0.9f;
	guiSquishPresets[3] = 1.0f;
	ChangeHUD3DDepthMode(HUD_3D_Depth_Modes::HUD_DEFAULT);
	ChangeGUI3DDepthMode(GUI_3D_Depth_Modes::GUI_DEFAULT);

	hotkeyCatch = false;
	toggleVRBoostHotkey = 0;
	edgePeekHotkey = 0;
	for (int i = 0; i < 5; i++)
	{
		guiHotkeys[i] = 0;
		hudHotkeys[i] = 0;
	}
	for (int i = 0; i < 16; i++)
		controls.xButtonsStatus[i] = false;
	
	
	return true;
}


/**
* VP menu helper to setup new frame.
* @param entryID [in, out] Provides the identifier by count of the menu entry.
* @param menuEntryCount [in] The number of menu entries.
***/
void D3DProxyDevice::VPMENU_NewFrame(UINT &entryID, UINT menuEntryCount)
{
	#ifdef SHOW_CALLS
		OutputDebugString("called VPMENU_NewFrame");
	#endif
	// set menu entry attraction
	menuAttraction.y = ((borderTopHeight-menuTop)/menuEntryHeight);
	menuAttraction.y -= (float)((UINT)menuAttraction.y);
	menuAttraction.y -= 0.5f;
	menuAttraction.y *= 2.0f;
	if ((menuVelocity.y>0.0f) && (menuAttraction.y<0.0f)) menuAttraction.y = 0.0f;
	if ((menuVelocity.y<0.0f) && (menuAttraction.y>0.0f)) menuAttraction.y = 0.0f;

	// handle border height
	if (borderTopHeight<menuTop)
	{
		borderTopHeight = menuTop;
		menuVelocity.y=0.0f;
		menuAttraction.y=0.0f;

	}
	if (borderTopHeight>(menuTop+(menuEntryHeight*(float)(menuEntryCount-1))))
	{
		borderTopHeight = menuTop+menuEntryHeight*(float)(menuEntryCount-1);
		menuVelocity.y=0.0f;
		menuAttraction.y=0.0f;
	}

	// get menu entry id
	float entry = (borderTopHeight-menuTop+(menuEntryHeight/3.0f))/menuEntryHeight;
	entryID = (UINT)entry;
	if (entryID >= menuEntryCount)
		OutputDebugString("Error in VP menu programming !");
}

/**
* VP menu main method.
***/
void D3DProxyDevice::VPMENU()
{
	#ifdef SHOW_CALLS
		OutputDebugString("called VPMENU");
	#endif
	switch (VPMENU_mode)
	{
	case D3DProxyDevice::MAINMENU:
		VPMENU_MainMenu();
		break;
	case D3DProxyDevice::WORLD_SCALE_CALIBRATION:
		VPMENU_WorldScale();
		break;
	case D3DProxyDevice::CONVERGENCE_ADJUSTMENT:
		VPMENU_Convergence();
		break;
	case D3DProxyDevice::HUD_CALIBRATION:
		VPMENU_HUD();
		break;
	case D3DProxyDevice::GUI_CALIBRATION:
		VPMENU_GUI();
		break;
	case D3DProxyDevice::OVERALL_SETTINGS:
		VPMENU_Settings();
		break;
	case D3DProxyDevice::VRBOOST_VALUES:
		VPMENU_VRBoostValues();
		break;
	case D3DProxyDevice::POS_TRACKING_SETTINGS:
		VPMENU_PosTracking();
		break;
	case D3DProxyDevice::COMFORT_MODE:
		VPMENU_ComfortMode();
		break;
	case D3DProxyDevice::DUCKANDCOVER_CONFIGURATION:
		VPMENU_DuckAndCover();
		break;
	case D3DProxyDevice::VPMENU_SHADER_ANALYZER_SUBMENU:
		VPMENU_ShaderSubMenu();
		break;
	case D3DProxyDevice::CHANGE_RULES_SCREEN:
		VPMENU_ChangeRules();
		break;
	case D3DProxyDevice::PICK_RULES_SCREEN:
		VPMENU_PickRules();
		break;
	case D3DProxyDevice::SHOW_SHADERS_SCREEN:
		VPMENU_ShowActiveShaders();
		break;
	}
}

/**
* Main Menu method.
***/
void D3DProxyDevice::VPMENU_MainMenu()
{
	#ifdef SHOW_CALLS
		OutputDebugString("called VPMENU_MainMenu");
	#endif
	UINT menuEntryCount = 12;
	if (config.game_type > 10000) menuEntryCount++;

	menuHelperRect.left = 0;
	menuHelperRect.top = 0;

	UINT entryID;
	VPMENU_NewFrame(entryID, menuEntryCount);
	UINT borderSelection = entryID;
	if (config.game_type <= 10000)
		entryID++;

	/**
	* ESCAPE : Set menu inactive and save the configuration.
	***/
	if (controls.Key_Down(VK_ESCAPE))
	{
		VPMENU_mode = VPMENU_Modes::INACTIVE;
		VPMENU_UpdateConfigSettings();
	}

	if ((controls.Key_Down(VK_RETURN) || controls.Key_Down(VK_RSHIFT) || (controls.xButtonsStatus[0x0c])) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		// shader analyzer sub menu
		if (entryID == 0)
		{
			VPMENU_mode = VPMENU_Modes::VPMENU_SHADER_ANALYZER_SUBMENU;
			menuVelocity.x+=2.0f;
		}
		// world scale
		if (entryID == 1)
		{
			VPMENU_mode = VPMENU_Modes::WORLD_SCALE_CALIBRATION;
			menuVelocity.x+=2.0f;
		}
		// hud calibration
		if (entryID == 2)
		{
			VPMENU_mode = VPMENU_Modes::CONVERGENCE_ADJUSTMENT;
			menuVelocity.x+=2.0f;
		}
		// hud calibration
		if (entryID == 3)
		{
			VPMENU_mode = VPMENU_Modes::HUD_CALIBRATION;
			menuVelocity.x+=2.0f;
		}
		// gui calibration
		if (entryID == 4)
		{
			VPMENU_mode = VPMENU_Modes::GUI_CALIBRATION;
			menuVelocity.x+=2.0f;
		}
		// overall settings
		if (entryID == 7)
		{
			VPMENU_mode = VPMENU_Modes::OVERALL_SETTINGS;
			menuVelocity.x+=2.0f;
		}	
		// vrboost settings
		if (entryID == 8)
		{
			VPMENU_mode = VPMENU_Modes::VRBOOST_VALUES;
			menuVelocity.x+=2.0f;
		}
		// position tracking settings
		if (entryID == 9)
		{
			VPMENU_mode = VPMENU_Modes::POS_TRACKING_SETTINGS;
			menuVelocity.x+=2.0f;
		}
		// comfort mode settings
		if (entryID == 10)
		{
			VPMENU_mode = VPMENU_Modes::COMFORT_MODE;
			menuVelocity.x+=2.0f;
		}
		// restore configuration
		if (entryID == 11)
		{
			// first, backup all strings
			std::string game_exe = std::string(config.game_exe);
			std::string shaderRulePath = std::string(config.shaderRulePath);
			std::string VRboostPath = std::string(config.VRboostPath);
			memcpy(&config, &m_configBackup, sizeof(ProxyHelper::ProxyConfig));
			config.game_exe = std::string(game_exe);
			config.shaderRulePath = std::string(shaderRulePath);
			config.VRboostPath = std::string(VRboostPath);
			VPMENU_UpdateDeviceSettings();
			VPMENU_UpdateConfigSettings();
			menuVelocity.x+=10.0f;
		}	
		// back to game
		if (entryID == 12)
		{
			VPMENU_mode = VPMENU_Modes::INACTIVE;
			VPMENU_UpdateConfigSettings();
		}
	}

	if (controls.Key_Down(VK_LEFT) || controls.Key_Down('J') || (controls.xInputState.Gamepad.sThumbLX<-8192))
	{
		// change hud scale 
		if ((entryID == 5) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			if (hud3DDepthMode > HUD_3D_Depth_Modes::HUD_DEFAULT)
				ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)(hud3DDepthMode-1));
			menuVelocity.x-=2.0f;
		}

		// change gui scale
		if ((entryID == 6) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			if (gui3DDepthMode > GUI_3D_Depth_Modes::GUI_DEFAULT)
				ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)(gui3DDepthMode-1));
			menuVelocity.x-=2.0f;
		}
	}

	if (controls.Key_Down(VK_RIGHT) || controls.Key_Down('L') || (controls.xInputState.Gamepad.sThumbLX>8192))
	{
		// change hud scale 
		if ((entryID == 5) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			if (hud3DDepthMode < HUD_3D_Depth_Modes::HUD_ENUM_RANGE-1)
				ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)(hud3DDepthMode+1));
			menuVelocity.x+=2.0f;
		}

		// change gui scale
		if ((entryID == 6) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			if (gui3DDepthMode < GUI_3D_Depth_Modes::GUI_ENUM_RANGE-1)
				ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)(gui3DDepthMode+1));
			menuVelocity.x+=2.0f;
		}

	}

	// output menu
	if (hudFont)
	{
		// adjust border
		float borderDrawingHeight = borderTopHeight;
		if ((menuVelocity.y < 1.0f) && (menuVelocity.y > -1.0f))
			borderTopHeight = menuTop+menuEntryHeight*(float)borderSelection;

		// draw border - total width due to shift correction
		D3DRECT rect;
		rect.x1 = (int)0; rect.x2 = (int)viewportWidth; rect.y1 = (int)borderTopHeight; rect.y2 = (int)(borderTopHeight+viewportHeight*0.04f);
		ClearEmptyRect(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255,255,128,128), 2);
		ClearEmptyRect(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255,255,128,128), 2);

		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		menuHelperRect.left = 650;
		menuHelperRect.top = 300;
		DrawTextShadowed(hudFont, hudMainMenu, "Vireio Perception ("APP_VERSION") Settings\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		rect.x1 = 0; rect.x2 = viewportWidth; rect.y1 = (int)(335*fScaleY); rect.y2 = (int)(340*fScaleY);
		Clear(1, &rect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,128,128), 0, 0);

		menuHelperRect.top += 50;  menuHelperRect.left += 150;
		if (config.game_type > 10000)
		{
			DrawTextShadowed(hudFont, hudMainMenu, "Activate Vireio Shader Analyzer\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			menuHelperRect.top += MENU_ITEM_SEPARATION;
		}
		DrawTextShadowed(hudFont, hudMainMenu, "World-Scale Calibration\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Convergence Adjustment\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "HUD Calibration\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "GUI Calibration\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION; float hudQSHeight = (float)menuHelperRect.top * fScaleY;
		DrawTextShadowed(hudFont, hudMainMenu, "HUD Quick Setting : \n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION; float guiQSHeight = (float)menuHelperRect.top * fScaleY;
		DrawTextShadowed(hudFont, hudMainMenu, "GUI Quick Setting : \n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Overall Settings\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "VRBoost Values\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Position Tracking Configuration\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Comfort Mode Configuration\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Restore Configuration\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to Game\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		
		// draw HUD quick setting rectangles
		rect.x1 = (int)(viewportWidth*0.57f); rect.x2 = (int)(viewportWidth*0.61f); rect.y1 = (int)hudQSHeight; rect.y2 = (int)(hudQSHeight+viewportHeight*0.027f);
		DrawSelection(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255, 128, 196, 128), (int)hud3DDepthMode, (int)HUD_3D_Depth_Modes::HUD_ENUM_RANGE);
		DrawSelection(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255, 128, 196, 128), (int)hud3DDepthMode, (int)HUD_3D_Depth_Modes::HUD_ENUM_RANGE);

		// draw GUI quick setting rectangles
		rect.x1 = (int)(viewportWidth*0.57f); rect.x2 = (int)(viewportWidth*0.61f); rect.y1 = (int)guiQSHeight; rect.y2 = (int)(guiQSHeight+viewportHeight*0.027f);
		DrawSelection(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255, 128, 196, 128), (int)gui3DDepthMode, (int)GUI_3D_Depth_Modes::GUI_ENUM_RANGE);
		DrawSelection(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255, 128, 196, 128), (int)gui3DDepthMode, (int)GUI_3D_Depth_Modes::GUI_ENUM_RANGE);

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;

		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		hudMainMenu->End();
	}
}

/**
* World Scale Calibration.
***/
void D3DProxyDevice::VPMENU_WorldScale()
{
	#ifdef SHOW_CALLS
		OutputDebugString("called VPMENU_WorldScale");
	#endif
	// base values
	float separationChange = 0.005f;
	static UINT gameXScaleUnitIndex = 0;

	// ensure that the attraction is set to zero
	// for non-menu-screens like this one
	menuAttraction.x = 0.0f;
	menuAttraction.y = 0.0f;

	// sort the game unit vector
	std::sort (m_gameXScaleUnits.begin(), m_gameXScaleUnits.end());

	// enter ? rshift ? increase gameXScaleUnitIndex
	if ((controls.Key_Down(VK_RETURN) || controls.Key_Down(VK_RSHIFT) || (controls.xButtonsStatus[0x0c])) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		if (controls.Key_Down(VK_LSHIFT))
		{
			if (gameXScaleUnitIndex>0) --gameXScaleUnitIndex;
		}
		else
			gameXScaleUnitIndex++;
		menuVelocity.x+=2.0f;
	}

	// game unit index out of range ?
	if ((gameXScaleUnitIndex != 0) && (gameXScaleUnitIndex >= m_gameXScaleUnits.size()))
		gameXScaleUnitIndex = m_gameXScaleUnits.size()-1;

	/**
	* ESCAPE : Set menu inactive and save the configuration.
	***/
	if (controls.Key_Down(VK_ESCAPE))
	{
		VPMENU_mode = VPMENU_Modes::INACTIVE;
		VPMENU_UpdateConfigSettings();
	}

	/**
	* LEFT : Decrease world scale (hold CTRL to lower speed, SHIFT to speed up)
	***/
	if((controls.Key_Down(VK_LEFT) || controls.Key_Down('J') || (controls.xInputState.Gamepad.sThumbLX<-8192)) && (menuVelocity.x == 0.0f))
	{
		if(controls.Key_Down(VK_LCONTROL)) {
			separationChange /= 10.0f;
		}
		else if(controls.Key_Down(VK_LSHIFT)) {
			separationChange *= 10.0f;
		} 
		else if(controls.Key_Down(VK_MENU))
		{
			separationChange /= 500.0f;
		}

		if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down('J'))
			m_spShaderViewAdjustment->ChangeWorldScale(separationChange * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f));
		else
			m_spShaderViewAdjustment->ChangeWorldScale(-separationChange);
		m_spShaderViewAdjustment->UpdateProjectionMatrices((float)stereoView->viewport.Width/(float)stereoView->viewport.Height);

		menuVelocity.x+=0.7f;
	}

	/**
	* RIGHT : Increase world scale (hold CTRL to lower speed, SHIFT to speed up)
	***/
	if((controls.Key_Down(VK_RIGHT) || controls.Key_Down('L') || (controls.xInputState.Gamepad.sThumbLX>8192)) && (menuVelocity.x == 0.0f))
	{
		if(controls.Key_Down(VK_LCONTROL)) {
			separationChange /= 10.0f;
		}
		else if(controls.Key_Down(VK_LSHIFT))
		{
			separationChange *= 10.0f;
		}
		else if(controls.Key_Down(VK_MENU))
		{
			separationChange /= 500.0f;
		}

		if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down('L'))
			m_spShaderViewAdjustment->ChangeWorldScale(separationChange * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f));
		else
			m_spShaderViewAdjustment->ChangeWorldScale(separationChange);
		m_spShaderViewAdjustment->UpdateProjectionMatrices((float)stereoView->viewport.Width/(float)stereoView->viewport.Height);

		menuVelocity.x+=0.7f;
	}

	// handle border height (=scrollbar scroll height)
	if (borderTopHeight<-64.0f)
		borderTopHeight = -64.0f;
	if (borderTopHeight>365.0f)
		borderTopHeight = 365.0f;

	if(hudFont){

		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		// standard hud size, will be scaled later to actual viewport
		char vcString[1024];
		int width = VPMENU_PIXEL_WIDTH;
		int height = VPMENU_PIXEL_HEIGHT;

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		// arbitrary formular... TODO !! find a more nifty solution
		float BlueLineCenterAsPercentage = m_spShaderViewAdjustment->HMDInfo()->GetLensXCenterOffset() * 0.2f;

		float horWidth = 0.15f;
		int beg = (int)(viewportWidth*(1.0f-horWidth)/2.0) + (int)(BlueLineCenterAsPercentage * viewportWidth * 0.25f);
		int end = (int)(viewportWidth*(0.5f+(horWidth/2.0f))) + (int)(BlueLineCenterAsPercentage * viewportWidth * 0.25f);

		int hashTop = (int)(viewportHeight  * 0.48f);
		int hashBottom = (int)(viewportHeight  * 0.52f);

		RECT rec2 = {(int)(width*0.27f), (int)(height*0.8f),width,height};
		sprintf_s(vcString, 1024, "Vireio Perception ("APP_VERSION") Settings - World Scale\n");
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec2, 0, D3DCOLOR_ARGB(255,255,255,255));

		// draw right line (using BaseDirect3DDevice9, since otherwise we have two lines)
		D3DRECT rec3 = {(int)(viewportWidth/2 + (-BlueLineCenterAsPercentage * viewportWidth * 0.25f))-1, 0,
			(int)(viewportWidth/2 + (-BlueLineCenterAsPercentage * viewportWidth * 0.25f))+1,viewportHeight };
		if (!config.swap_eyes)
			ClearRect(vireio::RenderPosition::Right, rec3, D3DCOLOR_ARGB(255,0,0,255));
		else
			ClearRect(vireio::RenderPosition::Left, rec3, D3DCOLOR_ARGB(255,0,0,255));

		// draw left line (using BaseDirect3DDevice9, since otherwise we have two lines)
		D3DRECT rec4 = {(int)(viewportWidth/2 + (BlueLineCenterAsPercentage * viewportWidth * 0.25f))-1, 0,
			(int)(viewportWidth/2 + (BlueLineCenterAsPercentage * viewportWidth * 0.25f))+1,viewportHeight };
		if (!config.swap_eyes)
			ClearRect(vireio::RenderPosition::Left, rec4, D3DCOLOR_ARGB(255,255,0,0));
		else
			ClearRect(vireio::RenderPosition::Right, rec4, D3DCOLOR_ARGB(255,255,0,0));

		// horizontal line
		D3DRECT rec5 = {beg, (viewportHeight /2)-1, end, (viewportHeight /2)+1 };
		if (!config.swap_eyes)
			ClearRect(vireio::RenderPosition::Left, rec5, D3DCOLOR_ARGB(255,0,0,255));
		else
			ClearRect(vireio::RenderPosition::Right, rec5, D3DCOLOR_ARGB(255,0,0,255));

		// hash lines
		int hashNum = 10;
		float hashSpace = horWidth*viewportWidth / (float)hashNum;
		for(int i=0; i<=hashNum; i++) {
			D3DRECT rec5 = {beg+(int)(i*hashSpace)-1, hashTop, beg+(int)(i*hashSpace)+1, hashBottom};
			if (!config.swap_eyes)
				ClearRect(vireio::RenderPosition::Left, rec5, D3DCOLOR_ARGB(255,255,255,0));
			else
				ClearRect(vireio::RenderPosition::Right, rec5, D3DCOLOR_ARGB(255,255,255,0));
		}

		rec2.left = (int)(width*0.35f);
		rec2.top = (int)(height*0.83f);
		sprintf_s(vcString, 1024, "World-Scale Calibration");
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec2, 0, D3DCOLOR_ARGB(255,255,255,255));

		RECT rec10 = {(int)(width*0.40f), (int)(height*0.57f),width,height};
		DrawTextShadowed(hudFont, hudMainMenu, "<- calibrate using Arrow Keys ->", -1, &rec10, 0, D3DCOLOR_ARGB(255,255,255,255));

		float gameUnit = m_spShaderViewAdjustment->WorldScale();

		// actual game unit chosen ... in case game has called SetTransform(>projection<);
		if (m_bProjectionTransformSet)
		{
			// get the scale the 
			float gameXScale = m_gameXScaleUnits[gameXScaleUnitIndex];

			// get the scale the driver projects
			D3DXMATRIX driverProjection = m_spShaderViewAdjustment->Projection();
			float driverXScale = driverProjection._11;

			// gameUnit = (driverWorldScale * driverXScale) /  gameXScale
			gameUnit = ((m_spShaderViewAdjustment->WorldScale()) * driverXScale ) / gameXScale;

			rec10.top = (int)(height*0.77f); rec10.left = (int)(width*0.45f);
			sprintf_s(vcString,"Actual Units %u/%u", gameXScaleUnitIndex, m_gameXScaleUnits.size());
			DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec10, 0, D3DCOLOR_ARGB(255,255,255,255));
		}

		//Column 1:
		//1 Game Unit = X Meters
		//1 Game Unit = X Centimeters
		//1 Game Unit = X Feet
		//1 Game Unit = X Inches
		//Column 2:
		//1 Meter = X Game Units
		//1 Centimeter = X Game Units
		//1 Foot = X Game Units
		//1 Inch = X Game Units
		rec10.top = (int)(height*0.6f); rec10.left = (int)(width*0.28f);
		float meters = 1 / gameUnit;
		sprintf_s(vcString,"1 Game Unit = %g Meters", meters);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec10, 0, D3DCOLOR_ARGB(255,255,255,255));
		rec10.top+=35;
		float centimeters = meters * 100.0f;
		sprintf_s(vcString,"1 Game Unit = %g CM", centimeters);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec10, 0, D3DCOLOR_ARGB(255,255,255,255));
		rec10.top+=35;
		float feet = meters * 3.2808399f;
		sprintf_s(vcString,"1 Game Unit = %g Feet", feet);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec10, 0, D3DCOLOR_ARGB(255,255,255,255));
		rec10.top+=35;
		float inches = feet * 12.0f;
		sprintf_s(vcString,"1 Game Unit = %g In.", inches);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec10, 0, D3DCOLOR_ARGB(255,255,255,255));

		RECT rec11 = {(int)(width*0.52f), (int)(height*0.6f),width,height};
		sprintf_s(vcString,"1 Meter      = %g Game Units", gameUnit);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec11, 0, D3DCOLOR_ARGB(255,255,255,255));
		rec11.top+=35;
		float gameUnitsToCentimeter =  gameUnit / 100.0f;
		sprintf_s(vcString,"1 CM         = %g Game Units", gameUnitsToCentimeter);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec11, 0, D3DCOLOR_ARGB(255,255,255,255));
		rec11.top+=35;
		float gameUnitsToFoot = gameUnit / 3.2808399f;
		sprintf_s(vcString,"1 Foot       = %g Game Units", gameUnitsToFoot);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec11, 0, D3DCOLOR_ARGB(255,255,255,255));
		rec11.top+=35;
		float gameUnitsToInches = gameUnit / 39.3700787f;
		sprintf_s(vcString,"1 Inch       = %g Game Units", gameUnitsToInches);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec11, 0, D3DCOLOR_ARGB(255,255,255,255));

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;

		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));  
		hudMainMenu->End();

		// draw description text box
		hudTextBox->Begin(D3DXSPRITE_ALPHABLEND);
		hudTextBox->SetTransform(&matScale);
		RECT rec8 = {620, (int)(borderTopHeight), 1300, 400};
		sprintf_s(vcString, 1024,
			"In the right eye view, walk up as close as\n"
			"possible to a 90 degree vertical object and\n"
			"align the BLUE vertical line with its edge.\n"
			"Good examples include a wall corner, a table\n"
			"corner, a square post, etc.  While looking at\n"
			"the left image, adjust the World View setting\n"
			"until the same object's edge is on the fourth\n"
			"notch in the >Negative Parallax< section (to\n"
			"the right of the RED line).  If objects go \n"
			"beyond this point, reduce the World Scale \n"
			"further.  Try to keep the BLUE line aligned\n"
			"while changing the World Scale.  Adjust \n"
			"further for comfort and game unit accuracy.\n"
			);
		DrawTextShadowed(hudFont, hudTextBox, vcString, -1, &rec8, 0, D3DCOLOR_ARGB(255,255,255,255));
		hudTextBox->Draw(NULL, &rec8, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));

		// draw description box scroll bar
		float scroll = (429.0f-borderTopHeight-64.0f)/429.0f;
		D3DRECT rec9 = {(int)(1300*fScaleX), 0, (int)(1320*fScaleX), (int)(400*fScaleY)};
		DrawScrollbar(vireio::RenderPosition::Left, rec9, D3DCOLOR_ARGB(255, 128, 196, 128), scroll, (int)(20*fScaleY));
		DrawScrollbar(vireio::RenderPosition::Right, rec9, D3DCOLOR_ARGB(255, 128, 196, 128), scroll, (int)(20*fScaleY));

		hudTextBox->End();
	}
}

/**
* Convergence Adjustment.
***/
void D3DProxyDevice::VPMENU_Convergence()
{
	#ifdef SHOW_CALLS
		OutputDebugString("called VPMENU_Convergence");
	#endif
	// base values
	float convergenceChange = 0.05f;

	// ensure that the attraction is set to zero
	// for non-menu-screens like this one
	menuAttraction.x = 0.0f;
	menuAttraction.y = 0.0f;

	/**
	* ESCAPE : Set menu inactive and save the configuration.
	***/
	if (controls.Key_Down(VK_ESCAPE))
	{
		VPMENU_mode = VPMENU_Modes::INACTIVE;
		VPMENU_UpdateConfigSettings();
	}

	/**
	* LEFT : Decrease convergence (hold CTRL to lower speed, SHIFT to speed up)
	***/
	if((controls.Key_Down(VK_LEFT) || controls.Key_Down('J') || (controls.xInputState.Gamepad.sThumbLX<-8192)) && (menuVelocity.x == 0.0f))
	{
		if(controls.Key_Down(VK_LCONTROL)) {
			convergenceChange /= 10.0f;
		}
		else if(controls.Key_Down(VK_LSHIFT)) {
			convergenceChange *= 10.0f;
		} 

		if (controls.xInputState.Gamepad.sThumbLX != 0  && !controls.Key_Down(VK_LEFT) && !controls.Key_Down('J'))
			m_spShaderViewAdjustment->ChangeConvergence(convergenceChange * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f));
		else
			m_spShaderViewAdjustment->ChangeConvergence(-convergenceChange);
		m_spShaderViewAdjustment->UpdateProjectionMatrices((float)stereoView->viewport.Width/(float)stereoView->viewport.Height);

		menuVelocity.x+=0.7f;
	}

	/**
	* RIGHT : Increase convergence (hold CTRL to lower speed, SHIFT to speed up)
	***/
	if((controls.Key_Down(VK_RIGHT) || controls.Key_Down('L') || (controls.xInputState.Gamepad.sThumbLX>8192)) && (menuVelocity.x == 0.0f))
	{
		if(controls.Key_Down(VK_LCONTROL)) {
			convergenceChange /= 10.0f;
		}
		else if(controls.Key_Down(VK_LSHIFT))
		{
			convergenceChange *= 10.0f;
		}

		if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down('L'))
			m_spShaderViewAdjustment->ChangeConvergence(convergenceChange * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f));
		else
			m_spShaderViewAdjustment->ChangeConvergence(convergenceChange);
		m_spShaderViewAdjustment->UpdateProjectionMatrices((float)stereoView->viewport.Width/(float)stereoView->viewport.Height);

		menuVelocity.x+=0.7f;
	}

	// handle border height (=scrollbar scroll height)
	if (borderTopHeight<-64.0f)
		borderTopHeight = -64.0f;
	if (borderTopHeight>365.0f)
		borderTopHeight = 365.0f;

	if(hudFont){

		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		// standard hud size, will be scaled later to actual viewport
		char vcString[1024];
		int width = VPMENU_PIXEL_WIDTH;
		int height = VPMENU_PIXEL_HEIGHT;

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		// arbitrary formular... TODO !! find a more nifty solution
		float BlueLineCenterAsPercentage = m_spShaderViewAdjustment->HMDInfo()->GetLensXCenterOffset() * 0.2f;

		float horWidth = 0.15f;
		int beg = (int)(viewportWidth*(1.0f-horWidth)/2.0) + (int)(BlueLineCenterAsPercentage * viewportWidth * 0.25f);
		int end = (int)(viewportWidth*(0.5f+(horWidth/2.0f))) + (int)(BlueLineCenterAsPercentage * viewportWidth * 0.25f);

		int hashTop = (int)(viewportHeight  * 0.48f);
		int hashBottom = (int)(viewportHeight  * 0.52f);

		RECT rec2 = {(int)(width*0.27f), (int)(height*0.8f),width,height};
		sprintf_s(vcString, 1024, "Vireio Perception ("APP_VERSION") Settings - Convergence\n");
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec2, 0, D3DCOLOR_ARGB(255,255,255,255));

		// draw right line (using BaseDirect3DDevice9, since otherwise we have two lines)
		D3DRECT rec3 = {(int)(viewportWidth/2 + (-BlueLineCenterAsPercentage * viewportWidth * 0.25f))-1, 0,
			(int)(viewportWidth/2 + (-BlueLineCenterAsPercentage * viewportWidth * 0.25f))+1,viewportHeight };
		if (!config.swap_eyes)
			ClearRect(vireio::RenderPosition::Right, rec3, D3DCOLOR_ARGB(255,0,0,255));
		else
			ClearRect(vireio::RenderPosition::Left, rec3, D3DCOLOR_ARGB(255,0,0,255));

		// draw left line (using BaseDirect3DDevice9, since otherwise we have two lines)
		D3DRECT rec4 = {(int)(viewportWidth/2 + (BlueLineCenterAsPercentage * viewportWidth * 0.25f))-1, 0,
			(int)(viewportWidth/2 + (BlueLineCenterAsPercentage * viewportWidth * 0.25f))+1,viewportHeight };
		if (!config.swap_eyes)
			ClearRect(vireio::RenderPosition::Left, rec4, D3DCOLOR_ARGB(255,0,0,255));
		else
			ClearRect(vireio::RenderPosition::Right, rec4, D3DCOLOR_ARGB(255,0,0,255));

		// horizontal line
		D3DRECT rec5 = {beg, (viewportHeight /2)-1, end, (viewportHeight /2)+1 };
		if (!config.swap_eyes)
			ClearRect(vireio::RenderPosition::Left, rec5, D3DCOLOR_ARGB(255,0,0,255));
		else
			ClearRect(vireio::RenderPosition::Right, rec5, D3DCOLOR_ARGB(255,0,0,255));

		// hash lines
		int hashNum = 10;
		float hashSpace = horWidth*viewportWidth / (float)hashNum;
		for(int i=0; i<=hashNum; i++) {
			D3DRECT rec5 = {beg+(int)(i*hashSpace)-1, hashTop, beg+(int)(i*hashSpace)+1, hashBottom};
			if (!config.swap_eyes)
				ClearRect(vireio::RenderPosition::Left, rec5, D3DCOLOR_ARGB(255,255,255,0));
			else
				ClearRect(vireio::RenderPosition::Right, rec5, D3DCOLOR_ARGB(255,255,255,0));
		}

		rec2.left = (int)(width*0.35f);
		rec2.top = (int)(height*0.83f);
		sprintf_s(vcString, 1024, "Convergence Adjustment");
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec2, 0, D3DCOLOR_ARGB(255,255,255,255));

		// output convergence
		RECT rec10 = {(int)(width*0.40f), (int)(height*0.57f),width,height};
		DrawTextShadowed(hudFont, hudMainMenu, "<- calibrate using Arrow Keys ->", -1, &rec10, 0, D3DCOLOR_ARGB(255,255,255,255));
		// Convergence Screen = X Meters = X Feet
		rec10.top = (int)(height*0.6f); rec10.left = (int)(width*0.385f);
		float meters = m_spShaderViewAdjustment->Convergence();
		sprintf_s(vcString,"Convergence Screen = %g Meters", meters);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec10, 0, D3DCOLOR_ARGB(255,255,255,255));
		rec10.top+=35;
		float centimeters = meters * 100.0f;
		sprintf_s(vcString,"Convergence Screen = %g CM", centimeters);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec10, 0, D3DCOLOR_ARGB(255,255,255,255));
		rec10.top+=35;
		float feet = meters * 3.2808399f;
		sprintf_s(vcString,"Convergence Screen = %g Feet", feet);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec10, 0, D3DCOLOR_ARGB(255,255,255,255));
		rec10.top+=35;
		float inches = feet * 12.0f;
		sprintf_s(vcString,"Convergence Screen = %g Inches", inches);
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &rec10, 0, D3DCOLOR_ARGB(255,255,255,255));

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;

		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));  
		hudMainMenu->End();

		// draw description text box
		hudTextBox->Begin(D3DXSPRITE_ALPHABLEND);
		hudTextBox->SetTransform(&matScale);
		RECT rec8 = {620, (int)(borderTopHeight), 1300, 400};
		sprintf_s(vcString, 1024,
			"Note that the Convergence Screens distance\n"
			"is measured in physical meters and should\n"
			"only be adjusted to match Your personal\n"
			"depth cognition after You calibrated the\n"
			"World Scale accordingly.\n"
			"In the right eye view, walk up as close as\n"
			"possible to a 90 degree vertical object and\n"
			"align the BLUE vertical line with its edge.\n"
			"Good examples include a wall corner, a table\n"
			"corner, a square post, etc.\n"
			);
		DrawTextShadowed(hudFont, hudTextBox, vcString, -1, &rec8, 0, D3DCOLOR_ARGB(255,255,255,255));
		hudTextBox->Draw(NULL, &rec8, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));

		// draw description box scroll bar
		float scroll = (429.0f-borderTopHeight-64.0f)/429.0f;
		D3DRECT rec9 = {(int)(1300*fScaleX), 0, (int)(1320*fScaleX), (int)(400*fScaleY)};
		DrawScrollbar(vireio::RenderPosition::Left, rec9, D3DCOLOR_ARGB(255, 128, 196, 128), scroll, (int)(20*fScaleY));
		DrawScrollbar(vireio::RenderPosition::Right, rec9, D3DCOLOR_ARGB(255, 128, 196, 128), scroll, (int)(20*fScaleY));

		hudTextBox->End();
	}
}

/**
* HUD Calibration.
***/
void D3DProxyDevice::VPMENU_HUD()
{
	#ifdef SHOW_CALLS
		OutputDebugString("called VPMENU_HUD");
	#endif
	UINT menuEntryCount = 10;

	menuHelperRect.left = 0;
	menuHelperRect.top = 0;

	UINT entryID;
	VPMENU_NewFrame(entryID, menuEntryCount);
	UINT borderSelection = entryID;

	if ((hotkeyCatch) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		for (int i = 0; i < 256; i++)
			if (controls.Key_Down(i) && controls.GetKeyName(i)!="-")
			{
				hotkeyCatch = false;
				int index = entryID-3;
				if ((index >=0) && (index <=4))
					hudHotkeys[index] = (byte)i;
			}
	}
	else
	{
		if (controls.Key_Down(VK_ESCAPE))
		{
			VPMENU_mode = VPMENU_Modes::INACTIVE;
			VPMENU_UpdateConfigSettings();
		}

		if ((controls.Key_Down(VK_RETURN) || controls.Key_Down(VK_RSHIFT) || (controls.xButtonsStatus[0x0c])) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			if ((entryID >= 3) && (entryID <= 7) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				hotkeyCatch = true;
				menuVelocity.x+=2.0f;
			}
			// back to main menu
			if (entryID == 8)
			{
				VPMENU_mode = VPMENU_Modes::MAINMENU;
				VPMENU_UpdateConfigSettings();
				menuVelocity.x+=2.0f;
			}
			// back to game
			if (entryID == 9)
			{
				VPMENU_mode = VPMENU_Modes::INACTIVE;
				VPMENU_UpdateConfigSettings();
			}
		}

		if (controls.Key_Down(VK_BACK))
		{
			if ((entryID >= 3) && (entryID <= 7) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				int index = entryID-3;
				if ((index >=0) && (index <=4))
					hudHotkeys[index] = 0;
				menuVelocity.x+=2.0f;
			}
		}

		if (controls.Key_Down(VK_LEFT) || controls.Key_Down('J') || (controls.xInputState.Gamepad.sThumbLX<-8192))
		{
			if ((entryID == 0) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (hud3DDepthMode > HUD_3D_Depth_Modes::HUD_DEFAULT)
					ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)(hud3DDepthMode-1));
				menuVelocity.x-=2.0f;
			}

			if ((entryID == 1) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down('J'))
					hudDistancePresets[(int)hud3DDepthMode]+=0.01f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					hudDistancePresets[(int)hud3DDepthMode]-=0.01f;
				ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)hud3DDepthMode);
				menuVelocity.x-=0.7f;
			}

			if ((entryID == 2) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down('J'))
					hud3DDepthPresets[(int)hud3DDepthMode]+=0.002f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					hud3DDepthPresets[(int)hud3DDepthMode]-=0.002f;
				ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)hud3DDepthMode);
				menuVelocity.x-=0.7f;
			}
		}

		if (controls.Key_Down(VK_RIGHT) || controls.Key_Down('L') || (controls.xInputState.Gamepad.sThumbLX>8192))
		{
			// change hud scale
			if ((entryID == 0) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (hud3DDepthMode < HUD_3D_Depth_Modes::HUD_ENUM_RANGE-1)
					ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)(hud3DDepthMode+1));
				menuVelocity.x+=2.0f;
			}

			if ((entryID == 1) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down('L'))
					hudDistancePresets[(int)hud3DDepthMode]+=0.01f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					hudDistancePresets[(int)hud3DDepthMode]+=0.01f;
				ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)hud3DDepthMode);
				menuVelocity.x+=0.7f;
			}

			if ((entryID == 2) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down('L'))
					hud3DDepthPresets[(int)hud3DDepthMode]+=0.002f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					hud3DDepthPresets[(int)hud3DDepthMode]+=0.002f;
				ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)hud3DDepthMode);
				menuVelocity.x+=0.7f;
			}
		}
	}
	// output menu
	if (hudFont)
	{
		// adjust border
		float borderDrawingHeight = borderTopHeight;
		if ((menuVelocity.y < 1.0f) && (menuVelocity.y > -1.0f))
			borderTopHeight = menuTop+menuEntryHeight*(float)borderSelection;

		// draw border - total width due to shift correction
		D3DRECT rect;
		rect.x1 = (int)0; rect.x2 = (int)viewportWidth; rect.y1 = (int)borderTopHeight; rect.y2 = (int)(borderTopHeight+viewportHeight*0.04f);
		ClearEmptyRect(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255,255,128,128), 2);
		ClearEmptyRect(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255,255,128,128), 2);

		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		menuHelperRect.left = 650;
		menuHelperRect.top = 300;
		DrawTextShadowed(hudFont, hudMainMenu, "Vireio Perception ("APP_VERSION") Settings - HUD\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		rect.x1 = 0; rect.x2 = viewportWidth; rect.y1 = (int)(335*fScaleY); rect.y2 = (int)(340*fScaleY);
		Clear(1, &rect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,128,128), 0, 0);

		menuHelperRect.top += 50;  menuHelperRect.left += 150; float hudQSHeight = (float)menuHelperRect.top * fScaleY;
		switch (hud3DDepthMode)
		{
		case D3DProxyDevice::HUD_DEFAULT:
			DrawTextShadowed(hudFont, hudMainMenu, "HUD : Default", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case D3DProxyDevice::HUD_SMALL:
			DrawTextShadowed(hudFont, hudMainMenu, "HUD : Small", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case D3DProxyDevice::HUD_LARGE:
			DrawTextShadowed(hudFont, hudMainMenu, "HUD : Large", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case D3DProxyDevice::HUD_FULL:
			DrawTextShadowed(hudFont, hudMainMenu, "HUD : Full", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		default:
			break;
		}
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		char vcString[128];
		sprintf_s(vcString,"HUD Distance : %g", RoundVireioValue(hudDistancePresets[(int)hud3DDepthMode]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"HUD's 3D Depth : %g", RoundVireioValue(hud3DDepthPresets[(int)hud3DDepthMode]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Switch< : ");
		std::string stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(hudHotkeys[0]));
		if ((hotkeyCatch) && (entryID==3))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Default< : ");
		stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(hudHotkeys[1]));
		if ((hotkeyCatch) && (entryID==4))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Small< : ");
		stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(hudHotkeys[2]));
		if ((hotkeyCatch) && (entryID==5))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Large< : ");
		stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(hudHotkeys[3]));
		if ((hotkeyCatch) && (entryID==6))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Full< : ");
		stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(hudHotkeys[4]));
		if ((hotkeyCatch) && (entryID==7))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to Main Menu", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to Game", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		// draw HUD quick setting rectangles
		rect.x1 = (int)(viewportWidth*0.52f); rect.x2 = (int)(viewportWidth*0.56f); rect.y1 = (int)hudQSHeight; rect.y2 = (int)(hudQSHeight+viewportHeight*0.027f);
		DrawSelection(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255, 128, 196, 128), (int)hud3DDepthMode, (int)HUD_3D_Depth_Modes::HUD_ENUM_RANGE);
		DrawSelection(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255, 128, 196, 128), (int)hud3DDepthMode, (int)HUD_3D_Depth_Modes::HUD_ENUM_RANGE);

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;

		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		hudMainMenu->End();
	}
}

/**
* GUI Calibration.
***/
void D3DProxyDevice::VPMENU_GUI()
{
	#ifdef SHOW_CALLS
		OutputDebugString("called VPMENU_GUI");
	#endif
	UINT menuEntryCount = 10;

	menuHelperRect.left = 0;
	menuHelperRect.top = 0;

	UINT entryID;
	VPMENU_NewFrame(entryID, menuEntryCount);
	UINT borderSelection = entryID;

	if ((hotkeyCatch) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		for (int i = 0; i < 256; i++)
			if (controls.Key_Down(i) && controls.GetKeyName(i)!="-")
			{
				hotkeyCatch = false;
				int index = entryID-3;
				if ((index >=0) && (index <=4))
					guiHotkeys[index] = (byte)i;
			}
	}
	else
	{
		if (controls.Key_Down(VK_ESCAPE))
		{
			VPMENU_mode = VPMENU_Modes::INACTIVE;
			VPMENU_UpdateConfigSettings();
		}

		if ((controls.Key_Down(VK_RETURN) || controls.Key_Down(VK_RSHIFT) || (controls.xButtonsStatus[0x0c])) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			if ((entryID >= 3) && (entryID <= 7) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				hotkeyCatch = true;
				menuVelocity.x+=2.0f;
			}
			// back to main menu
			if (entryID == 8)
			{
				VPMENU_mode = VPMENU_Modes::MAINMENU;
				menuVelocity.x+=2.0f;
			}
			// back to game
			if (entryID == 9)
			{
				VPMENU_mode = VPMENU_Modes::INACTIVE;
				VPMENU_UpdateConfigSettings();
			}
		}

		if (controls.Key_Down(VK_BACK))
		{
			if ((entryID >= 3) && (entryID <= 7) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				int index = entryID-3;
				if ((index >=0) && (index <=4))
					guiHotkeys[index] = 0;
				menuVelocity.x+=2.0f;
			}
		}

		if (controls.Key_Down(VK_LEFT) || controls.Key_Down('J') || (controls.xInputState.Gamepad.sThumbLX<-8192))
		{
			if ((entryID == 0) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (gui3DDepthMode > GUI_3D_Depth_Modes::GUI_DEFAULT)
					ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)(gui3DDepthMode-1));
				menuVelocity.x-=2.0f;
			}

			if ((entryID == 1) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down('J'))
					guiSquishPresets[(int)gui3DDepthMode]+=0.01f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					guiSquishPresets[(int)gui3DDepthMode]-=0.01f;
				ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)gui3DDepthMode);
				menuVelocity.x-=0.7f;
			}

			if ((entryID == 2) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down('J'))
					gui3DDepthPresets[(int)gui3DDepthMode]+=0.002f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					gui3DDepthPresets[(int)gui3DDepthMode]-=0.002f;
				ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)gui3DDepthMode);
				menuVelocity.x-=0.7f;
			}
		}

		if (controls.Key_Down(VK_RIGHT) || controls.Key_Down('L') || (controls.xInputState.Gamepad.sThumbLX>8192))
		{
			// change gui scale
			if ((entryID == 0) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (gui3DDepthMode < GUI_3D_Depth_Modes::GUI_ENUM_RANGE-1)
					ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)(gui3DDepthMode+1));
				menuVelocity.x+=2.0f;
			}

			if ((entryID == 1) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down('L'))
					guiSquishPresets[(int)gui3DDepthMode]+=0.01f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					guiSquishPresets[(int)gui3DDepthMode]+=0.01f;
				ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)gui3DDepthMode);
				menuVelocity.x+=0.7f;
			}

			if ((entryID == 2) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down('L'))
					gui3DDepthPresets[(int)gui3DDepthMode]+=0.002f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					gui3DDepthPresets[(int)gui3DDepthMode]+=0.002f;
				ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)gui3DDepthMode);
				menuVelocity.x+=0.5;
			}
		}
	}
	// output menu
	if (hudFont)
	{
		// adjust border
		float borderDrawingHeight = borderTopHeight;
		if ((menuVelocity.y < 1.0f) && (menuVelocity.y > -1.0f))
			borderTopHeight = menuTop+menuEntryHeight*(float)borderSelection;

		// draw border - total width due to shift correction
		D3DRECT rect;
		rect.x1 = (int)0; rect.x2 = (int)viewportWidth; rect.y1 = (int)borderTopHeight; rect.y2 = (int)(borderTopHeight+viewportHeight*0.04f);
		ClearEmptyRect(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255,255,128,128), 2);
		ClearEmptyRect(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255,255,128,128), 2);

		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		menuHelperRect.left = 650;
		menuHelperRect.top = 300;
		DrawTextShadowed(hudFont, hudMainMenu, "Vireio Perception ("APP_VERSION") Settings - GUI\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		rect.x1 = 0; rect.x2 = viewportWidth; rect.y1 = (int)(335*fScaleY); rect.y2 = (int)(340*fScaleY);
		Clear(1, &rect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,128,128), 0, 0);

		menuHelperRect.top += 50;  menuHelperRect.left +=150; float guiQSHeight = (float)menuHelperRect.top * fScaleY;
		switch (gui3DDepthMode)
		{
		case D3DProxyDevice::GUI_DEFAULT:
			DrawTextShadowed(hudFont, hudMainMenu, "GUI : Default", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case D3DProxyDevice::GUI_SMALL:
			DrawTextShadowed(hudFont, hudMainMenu, "GUI : Small", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case D3DProxyDevice::GUI_LARGE:
			DrawTextShadowed(hudFont, hudMainMenu, "GUI : Large", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case D3DProxyDevice::GUI_FULL:
			DrawTextShadowed(hudFont, hudMainMenu, "GUI : Full", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		default:
			break;
		}
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		char vcString[128];
		sprintf_s(vcString,"GUI Size : %g", RoundVireioValue(guiSquishPresets[(int)gui3DDepthMode]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"GUI's 3D Depth : %g", RoundVireioValue(gui3DDepthPresets[(int)gui3DDepthMode]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Switch< : ");
		std::string stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(guiHotkeys[0]));
		if ((hotkeyCatch) && (entryID==3))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Default< : ");
		stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(guiHotkeys[1]));
		if ((hotkeyCatch) && (entryID==4))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Small< : ");
		stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(guiHotkeys[2]));
		if ((hotkeyCatch) && (entryID==5))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Large< : ");
		stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(guiHotkeys[3]));
		if ((hotkeyCatch) && (entryID==6))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Full< : ");
		stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(guiHotkeys[4]));
		if ((hotkeyCatch) && (entryID==7))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to Main Menu", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to Game", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		// draw GUI quick setting rectangles
		rect.x1 = (int)(viewportWidth*0.52f); rect.x2 = (int)(viewportWidth*0.56f); rect.y1 = (int)guiQSHeight; rect.y2 = (int)(guiQSHeight+viewportHeight*0.027f);
		DrawSelection(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255, 128, 196, 128), (int)gui3DDepthMode, (int)GUI_3D_Depth_Modes::GUI_ENUM_RANGE);
		DrawSelection(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255, 128, 196, 128), (int)gui3DDepthMode, (int)GUI_3D_Depth_Modes::GUI_ENUM_RANGE);

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;

		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		hudMainMenu->End();
	}
}

/**
* Settings.
***/
void D3DProxyDevice::VPMENU_Settings()
{
	#ifdef SHOW_CALLS
		OutputDebugString("called VPMENU_Settings");
	#endif

	//Use enumeration for menu items to avoid confusion
	enum 
	{
		SWAP_EYES,
		IPD_OFFSET,
		Y_OFFSET,
		DISTORTION_SCALE,
		YAW_MULT,
		PITCH_MULT,
		ROLL_MULT,
		RESET_MULT,
		ROLL_ENABLED,
		FORCE_MOUSE_EMU,
		TOGGLE_VRBOOST,
		HOTKEY_VRBOOST,
		HOTKEY_EDGEPEEK,
		BACK_VPMENU,
		BACK_GAME,
		NUM_MENU_ITEMS		
	};

	UINT menuEntryCount = NUM_MENU_ITEMS;

	menuHelperRect.left = 0;
	menuHelperRect.top = 0;

	UINT entryID;
	VPMENU_NewFrame(entryID, menuEntryCount);
	UINT borderSelection = entryID;

	if ((hotkeyCatch) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		for (int i = 0; i < 256; i++)
			if (controls.Key_Down(i) && controls.GetKeyName(i)!="-")
			{
				hotkeyCatch = false;
				if(entryID == HOTKEY_VRBOOST)
					toggleVRBoostHotkey = (byte)i;
				else
					edgePeekHotkey = (byte)i;
			}
	}
	else
	{
		/**
		* ESCAPE : Set menu inactive and save the configuration.
		***/
		if (controls.Key_Down(VK_ESCAPE))
		{
			VPMENU_mode = VPMENU_Modes::INACTIVE;
			VPMENU_UpdateConfigSettings();
		}

		if ((controls.Key_Down(VK_RETURN) || controls.Key_Down(VK_RSHIFT) || (controls.xButtonsStatus[0x0c])) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			// swap eyes
			if (entryID == SWAP_EYES)
			{
				stereoView->swapEyes = !stereoView->swapEyes;
				menuVelocity.x += 4.0f;
			}
			// screenshot
			/*if (entryID == STEREO_SCREENSHOTS)
			{
				// render 3 frames to get screenshots without menu
				screenshot = 3;
				VPMENU_mode = VPMENU_Modes::INACTIVE;
			}*/
			// reset multipliers
			if (entryID == RESET_MULT)
			{
				tracker->multiplierYaw = 25.0f;
				tracker->multiplierPitch = 25.0f;
				tracker->multiplierRoll = 1.0f;
				menuVelocity.x += 4.0f;
			}

			// update roll implementation
			if (entryID == ROLL_ENABLED)
			{
				config.rollImpl = (config.rollImpl+1) % 3;
				m_spShaderViewAdjustment->SetRollImpl(config.rollImpl);
				menuVelocity.x += 4.0f;
			}

			// force mouse emulation
			if (entryID == FORCE_MOUSE_EMU)
			{
				m_bForceMouseEmulation = !m_bForceMouseEmulation;

				if ((m_bForceMouseEmulation) && (tracker->getStatus() >= MTS_OK) && (!m_bSurpressHeadtracking))
					tracker->setMouseEmulation(true);

				if ((!m_bForceMouseEmulation) && (hmVRboost) && (m_VRboostRulesPresent)  && (tracker->getStatus() >= MTS_OK))
					tracker->setMouseEmulation(false);

				menuVelocity.x += 4.0f;
			}
			// Toggle VRBoost
			if (entryID == TOGGLE_VRBOOST)
			{
				if (hmVRboost!=NULL)
				{
					m_pVRboost_ReleaseAllMemoryRules();
					m_bVRBoostToggle = !m_bVRBoostToggle;
					if (tracker->getStatus() >= MTS_OK)
						tracker->resetOrientationAndPosition();
					menuVelocity.x+=2.0f;
				}
			}
			// VRBoost hotkey
			if (entryID == HOTKEY_VRBOOST)
			{
				hotkeyCatch = true;
				menuVelocity.x+=2.0f;
			}
			// VRBoost hotkey
			if (entryID == HOTKEY_EDGEPEEK)
			{
				hotkeyCatch = true;
				menuVelocity.x+=2.0f;
			}
			// back to main menu
			if (entryID == BACK_VPMENU)
			{
				VPMENU_mode = VPMENU_Modes::MAINMENU;
				VPMENU_UpdateConfigSettings();
				menuVelocity.x+=2.0f;
			}
			// back to game
			if (entryID == BACK_GAME)
			{
				VPMENU_mode = VPMENU_Modes::INACTIVE;
				VPMENU_UpdateConfigSettings();
			}
		}

		if (controls.Key_Down(VK_BACK))
		{
			if ((entryID >= DISTORTION_SCALE) && (entryID <= ROLL_MULT) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
			{
				int index = entryID-3;
				if ((index >=0) && (index <=4))
					guiHotkeys[index] = 0;
				menuVelocity.x+=2.0f;
			}
		}

		if (controls.Key_Down(VK_BACK) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			// ipd-offset
			if (entryID == IPD_OFFSET)
 			{
				this->stereoView->IPDOffset = 0.0f;
				this->stereoView->PostReset();
				menuVelocity.x += 0.7f;
			}
			//y offset
			if (entryID == Y_OFFSET)
 			{
				this->stereoView->YOffset = 0.0f;
				this->stereoView->PostReset();
				menuVelocity.x += 0.7f;
			}
			// distortion
			if (entryID == DISTORTION_SCALE)
			{
				this->stereoView->DistortionScale = 0.0f;
				this->stereoView->PostReset();
				menuVelocity.x += 0.7f;
			}
			// reset hotkey
			if (entryID == HOTKEY_VRBOOST)
			{
				toggleVRBoostHotkey = 0;
				menuVelocity.x+=2.0f;
			}
			// reset hotkey
			if (entryID == HOTKEY_EDGEPEEK)
			{
				edgePeekHotkey = 0;
				menuVelocity.x+=2.0f;
			}
		}

		if ((controls.Key_Down(VK_LEFT) || controls.Key_Down('J') || (controls.xInputState.Gamepad.sThumbLX<-8192)) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			// swap eyes
			if (entryID == SWAP_EYES)
			{
				stereoView->swapEyes = !stereoView->swapEyes;
				menuVelocity.x-=2.0f;
			}
			// ipd-offset
			if (entryID == IPD_OFFSET)
 			{
 				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down('J'))
				{
					if (this->stereoView->IPDOffset > 0.1f)
						this->stereoView->IPDOffset -= 0.001f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				}
				else
				{
					if (this->stereoView->IPDOffset > -0.1f)
						this->stereoView->IPDOffset -= 0.001f;
				}
				this->stereoView->PostReset();
				menuVelocity.x -= 0.7f;
			}
			// y-offset
			if (entryID == Y_OFFSET)
 			{
 				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down('J'))
				{
					if (this->stereoView->YOffset > 0.1f)
						this->stereoView->YOffset += 0.001f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				}
				else
				{
					if (this->stereoView->YOffset > -0.1f)
						this->stereoView->YOffset -= 0.001f;
				}
				this->stereoView->PostReset();
				menuVelocity.x -= 0.7f;
			}
			// distortion
			if (entryID == DISTORTION_SCALE)
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down('J'))
					this->stereoView->DistortionScale -= 0.01f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					this->stereoView->DistortionScale -= 0.01f;
				this->stereoView->PostReset();
				menuVelocity.x -= 0.7f;
			}
			// yaw multiplier
			if (entryID == YAW_MULT)
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down('J'))
					tracker->multiplierYaw += 0.5f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					tracker->multiplierYaw -= 0.5f;
				menuVelocity.x -= 0.7f;
			}
			// pitch multiplier
			if (entryID == PITCH_MULT)
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down('J'))
					tracker->multiplierPitch += 0.5f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					tracker->multiplierPitch -= 0.5f;
				menuVelocity.x -= 0.7f;
			}
			// roll multiplier
			if (entryID == ROLL_MULT)
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down('J'))
					tracker->multiplierRoll += 0.05f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					tracker->multiplierRoll -= 0.05f;
				menuVelocity.x -= 0.7f;
			}

			// mouse emulation
			if (entryID == FORCE_MOUSE_EMU)
			{
				m_bForceMouseEmulation = false;

				if ((hmVRboost) && (m_VRboostRulesPresent) && (tracker->getStatus() >= MTS_OK))
					tracker->setMouseEmulation(false);

				menuVelocity.x-=2.0f;
			}
		}

		if ((controls.Key_Down(VK_RIGHT) || controls.Key_Down('L') || (controls.xInputState.Gamepad.sThumbLX>8192)) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			// swap eyes
			if (entryID == SWAP_EYES)
			{
				stereoView->swapEyes = !stereoView->swapEyes;
				menuVelocity.x-=2.0f;
			}
			// ipd-offset
			if (entryID == IPD_OFFSET)
 			{
 				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down('L'))
				{
					if (this->stereoView->IPDOffset < 0.1f)
						this->stereoView->IPDOffset += 0.001f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				}
				else
				{
					if (this->stereoView->IPDOffset < 0.1f)
						this->stereoView->IPDOffset += 0.001f;
				}
				this->stereoView->PostReset();
				menuVelocity.x += 0.7f;
			}
			// y-offset
			if (entryID == Y_OFFSET)
 			{
 				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down('L'))
				{
					if (this->stereoView->YOffset < 0.1f)
						this->stereoView->YOffset += 0.001f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				}
				else
				{
					if (this->stereoView->YOffset < 0.1f)
						this->stereoView->YOffset += 0.001f;
				}
				this->stereoView->PostReset();
				menuVelocity.x += 0.7f;
			}
			if (entryID == DISTORTION_SCALE)
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down('L'))
					this->stereoView->DistortionScale += 0.01f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					this->stereoView->DistortionScale += 0.01f;
				this->stereoView->PostReset();
				menuVelocity.x += 0.7f;
			}
			// yaw multiplier
			if (entryID == YAW_MULT)
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0  && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down('L'))
					tracker->multiplierYaw += 0.5f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					tracker->multiplierYaw += 0.5f;
				menuVelocity.x += 0.7f;
			}
			// pitch multiplier
			if (entryID == PITCH_MULT)
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0  && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down('L'))
					tracker->multiplierPitch += 0.5f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					tracker->multiplierPitch += 0.5f;
				menuVelocity.x += 0.7f;
			}
			// roll multiplier
			if (entryID == ROLL_MULT)
			{
				if (controls.xInputState.Gamepad.sThumbLX != 0  && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down('L'))
					tracker->multiplierRoll += 0.05f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
				else
					tracker->multiplierRoll += 0.05f;
				menuVelocity.x += 0.7f;
			}
			// mouse emulation
			if (entryID == FORCE_MOUSE_EMU)
			{
				if(tracker->getStatus() >= MTS_OK)
				{
					tracker->setMouseEmulation(true);
					m_bForceMouseEmulation = true;
				}

				menuVelocity.x-=2.0f;
			}
		}
	}
	// output menu
	if (hudFont)
	{
		// adjust border
		float borderDrawingHeight = borderTopHeight;
		if ((menuVelocity.y < 1.0f) && (menuVelocity.y > -1.0f))
			borderTopHeight = menuTop+menuEntryHeight*(float)borderSelection;

		// draw border - total width due to shift correction
		D3DRECT rect;
		rect.x1 = (int)0; rect.x2 = (int)viewportWidth; rect.y1 = (int)borderTopHeight; rect.y2 = (int)(borderTopHeight+viewportHeight*0.04f);
		ClearEmptyRect(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255,255,128,128), 2);
		ClearEmptyRect(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255,255,128,128), 2);

		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		menuHelperRect.left = 650;
		menuHelperRect.top = 300;
		DrawTextShadowed(hudFont, hudMainMenu, "Vireio Perception ("APP_VERSION") Settings - General\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		rect.x1 = 0; rect.x2 = viewportWidth; rect.y1 = (int)(335*fScaleY); rect.y2 = (int)(340*fScaleY);
		Clear(1, &rect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,128,128), 0, 0);

		menuHelperRect.top += 50;  menuHelperRect.left += 150; float guiQSHeight = (float)menuHelperRect.top * fScaleY;
		switch (stereoView->swapEyes)
		{
		case true:
			DrawTextShadowed(hudFont, hudMainMenu, "Swap Eyes : True", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case false:
			DrawTextShadowed(hudFont, hudMainMenu, "Swap Eyes : False", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		}
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		char vcString[128];
		sprintf_s(vcString,"IPD-Offset : %1.3f", RoundVireioValue(this->stereoView->IPDOffset));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Y-Offset : %1.3f", RoundVireioValue(this->stereoView->YOffset));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Distortion Scale : %g", RoundVireioValue(this->stereoView->DistortionScale));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		//DrawTextShadowed(hudFont, hudMainMenu, "Stereo Screenshots", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		//menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Yaw multiplier : %g", RoundVireioValue(tracker->multiplierYaw));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Pitch multiplier : %g", RoundVireioValue(tracker->multiplierPitch));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Roll multiplier : %g", RoundVireioValue(tracker->multiplierRoll));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Reset Multipliers", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		switch (m_spShaderViewAdjustment->RollImpl())
		{
		case 0:
			DrawTextShadowed(hudFont, hudMainMenu, "Roll : Not Enabled", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case 1:
			DrawTextShadowed(hudFont, hudMainMenu, "Roll : Matrix Translation", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case 2:
			DrawTextShadowed(hudFont, hudMainMenu, "Roll : Pixel Shader", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		}
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		switch (m_bForceMouseEmulation)
		{
		case true:
			DrawTextShadowed(hudFont, hudMainMenu, "Force Mouse Emulation HT : True", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case false:
			DrawTextShadowed(hudFont, hudMainMenu, "Force Mouse Emulation HT : False", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		}
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		switch (m_bVRBoostToggle)
		{
		case true:
			DrawTextShadowed(hudFont, hudMainMenu, "Toggle VRBoost : On", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 64, 255, 64));
			break;
		case false:
			DrawTextShadowed(hudFont, hudMainMenu, "Toggle VRBoost : Off", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 128, 128));
			break;
		}
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Toggle VRBoost< : ");
		std::string stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(toggleVRBoostHotkey));
		if ((hotkeyCatch) && (entryID==11))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Hotkey >Disconnected Screen< : ");
		stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(edgePeekHotkey));
		if ((hotkeyCatch) && (entryID==12))
			stdString = "Press the desired key.";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to Main Menu", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to Game", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;

		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		hudMainMenu->End();
	}
}


/**
* Positional Tracking Settings.
***/
void D3DProxyDevice::VPMENU_PosTracking()
{
	#ifdef SHOW_CALLS
		OutputDebugString("called VPMENU_PosTracking");
	#endif

	enum
	{
		TOGGLE_TRACKING,
		TRACKING_MULT,
		TRACKING_MULT_X,
		TRACKING_MULT_Y,
		TRACKING_MULT_Z,
		RESET_HMD,
		DUCKANDCOVER_CONFIG,
		BACK_VPMENU,
		BACK_GAME,
		NUM_MENU_ITEMS
	};

	UINT menuEntryCount = NUM_MENU_ITEMS;

	menuHelperRect.left = 0;
	menuHelperRect.top = 0;

	UINT entryID;
	VPMENU_NewFrame(entryID, menuEntryCount);
	UINT borderSelection = entryID;

	/**
	* ESCAPE : Set menu inactive and save the configuration.
	***/
	if (controls.Key_Down(VK_ESCAPE))
	{
		VPMENU_mode = VPMENU_Modes::INACTIVE;
		VPMENU_UpdateConfigSettings();
	}

	if ((controls.Key_Down(VK_RETURN) || controls.Key_Down(VK_RSHIFT) || (controls.xButtonsStatus[0x0c])) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		// toggle position tracking
		if (entryID == TOGGLE_TRACKING)
		{
			m_bPosTrackingToggle = !m_bPosTrackingToggle;

			if (!m_bPosTrackingToggle)
				m_spShaderViewAdjustment->UpdatePosition(0.0f, 0.0f, 0.0f);

			menuVelocity.x += 3.0f;
		}

		// ientientation
		if (entryID == RESET_HMD)
		{
			tracker->resetOrientationAndPosition();
			menuVelocity.x += 3.0f;
		}

		if (entryID == DUCKANDCOVER_CONFIG)
		{
			VPMENU_mode = VPMENU_Modes::DUCKANDCOVER_CONFIGURATION;
			menuVelocity.x += 3.0f;
		}

		// back to main menu
		if (entryID == BACK_VPMENU)
		{
			VPMENU_mode = VPMENU_Modes::MAINMENU;
			VPMENU_UpdateConfigSettings();
			menuVelocity.x+=2.0f;
		}

		// back to game
		if (entryID == BACK_GAME)
		{
			VPMENU_mode = VPMENU_Modes::INACTIVE;
			VPMENU_UpdateConfigSettings();
		}
	}

	if (controls.Key_Down(VK_BACK))
	{
		if ((entryID == TRACKING_MULT) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			config.position_multiplier = 1.0f;
			menuVelocity.x+=1.0f;
		}
		if ((entryID == TRACKING_MULT_X) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			config.position_x_multiplier = 2.0f;
			menuVelocity.x+=1.0f;
		}
		if ((entryID == TRACKING_MULT_Y) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			config.position_y_multiplier = 2.5f;
			menuVelocity.x+=1.0f;
		}
		if ((entryID == TRACKING_MULT_Z) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			config.position_z_multiplier = 0.5f;
			menuVelocity.x+=1.0f;
		}
	}

	if ((controls.Key_Down(VK_LEFT) || controls.Key_Down('J') || (controls.xInputState.Gamepad.sThumbLX<-8192)) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		// overall position multiplier
		if (entryID == TRACKING_MULT)
		{
			if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down('J'))
				config.position_multiplier += 0.01f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
			else
				config.position_multiplier -= 0.01f;
			menuVelocity.x += 1.0f;
		}

		// overall position multiplier
		if (entryID == TRACKING_MULT_X)
		{
			if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down('J'))
				config.position_x_multiplier += 0.01f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
			else
				config.position_x_multiplier -= 0.01f;
			menuVelocity.x -= 0.6f;
		}

		// overall position multiplier
		if (entryID == TRACKING_MULT_Y)
		{
			if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down('J'))
				config.position_y_multiplier += 0.01f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
			else
				config.position_y_multiplier -= 0.01f;
			menuVelocity.x -= 0.6f;
		}

		// overall position multiplier
		if (entryID == TRACKING_MULT_Z)
		{
			if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_LEFT) && !controls.Key_Down('J'))
				config.position_z_multiplier += 0.01f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
			else
				config.position_z_multiplier -= 0.01f;
			menuVelocity.x -= 0.6f;
		}
	}


	if ((controls.Key_Down(VK_RIGHT) || controls.Key_Down('L') || (controls.xInputState.Gamepad.sThumbLX>8192)) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		// overall position multiplier
		if (entryID == TRACKING_MULT)
		{
			if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down('J'))
				config.position_multiplier += 0.01f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
			else
				config.position_multiplier += 0.01f;
			menuVelocity.x -= 0.6f;
		}

		// overall position multiplier
		if (entryID == TRACKING_MULT_X)
		{
			if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down('J'))
				config.position_x_multiplier += 0.01f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
			else
				config.position_x_multiplier += 0.01f;
			menuVelocity.x -= 0.6f;
		}

		// overall position multiplier
		if (entryID == TRACKING_MULT_Y)
		{
			if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down('J'))
				config.position_y_multiplier += 0.01f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
			else
				config.position_y_multiplier += 0.01f;
			menuVelocity.x -= 0.6f;
		}

		// overall position multiplier
		if (entryID == TRACKING_MULT_Z)
		{
			if (controls.xInputState.Gamepad.sThumbLX != 0 && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down('J'))
				config.position_z_multiplier += 0.01f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
			else
				config.position_z_multiplier += 0.01f;
			menuVelocity.x -= 0.6f;
		}
	}


	// output menu
	if (hudFont)
	{
		// adjust border
		float borderDrawingHeight = borderTopHeight;
		if ((menuVelocity.y < 1.0f) && (menuVelocity.y > -1.0f))
			borderTopHeight = menuTop+menuEntryHeight*(float)borderSelection;

		// draw border - total width due to shift correction
		D3DRECT rect;
		rect.x1 = (int)0; rect.x2 = (int)viewportWidth; rect.y1 = (int)borderTopHeight; rect.y2 = (int)(borderTopHeight+viewportHeight*0.04f);
		ClearEmptyRect(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255,255,128,128), 2);
		ClearEmptyRect(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255,255,128,128), 2);

		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		menuHelperRect.left = 650;
		menuHelperRect.top = 300;
		DrawTextShadowed(hudFont, hudMainMenu, "Vireio Perception ("APP_VERSION") Settings - Positional Tracking\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		rect.x1 = 0; rect.x2 = viewportWidth; rect.y1 = (int)(335*fScaleY); rect.y2 = (int)(340*fScaleY);
		Clear(1, &rect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,128,128), 0, 0);

		menuHelperRect.top += 50;  menuHelperRect.left += 150; float guiQSHeight = (float)menuHelperRect.top * fScaleY;
		char vcString[128];
		switch (m_bPosTrackingToggle)
		{
		case true:
			DrawTextShadowed(hudFont, hudMainMenu, "Positional Tracking (CTRL + P) : On", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 64, 255, 64));
			break;
		case false:
			DrawTextShadowed(hudFont, hudMainMenu, "Positional Tracking (CTRL + P) : Off", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 128, 128));
			break;
		}
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Position Tracking multiplier : %g", RoundVireioValue(config.position_multiplier));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Position X-Tracking multiplier : %g", RoundVireioValue(config.position_x_multiplier));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Position Y-Tracking multiplier : %g", RoundVireioValue(config.position_y_multiplier));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Position Z-Tracking multiplier : %g", RoundVireioValue(config.position_z_multiplier));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Reset HMD Orientation (LSHIFT + R)", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Duck-and-Cover Configuration", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to Main Menu", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to Game", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;

		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		hudMainMenu->End();
	}
}

/**
* configure DuckAndCover.
***/
void D3DProxyDevice::VPMENU_DuckAndCover()
{
	#ifdef SHOW_CALLS
		OutputDebugString("called VPMENU_DuckAndCover");
	#endif

	enum
	{
		CROUCH_TOGGLE,
		CROUCH_KEY,
		PRONE_TOGGLE,
		PRONE_KEY,
		JUMP_ENABLED,
		JUMP_KEY,
		DUCKANDCOVER_CALIBRATE,
		DUCKANDCOVER_MODE,
		BACK_VPMENU,
		BACK_GAME,
		NUM_MENU_ITEMS
	};

	UINT menuEntryCount = NUM_MENU_ITEMS;

	menuHelperRect.left = 0;
	menuHelperRect.top = 0;

	UINT entryID;
	VPMENU_NewFrame(entryID, menuEntryCount);
	UINT borderSelection = entryID;
	controls.UpdateXInputs();

	if ((hotkeyCatch) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		for (int i = 0; i < 256; i++)
			if (controls.Key_Down(i) && controls.GetKeyName(i)!="-")
			{
				hotkeyCatch = false;
				if(entryID == CROUCH_KEY)
					m_DuckAndCover.crouchKey = (byte)i;
				else if(entryID == PRONE_KEY)
					m_DuckAndCover.proneKey = (byte)i;
				else if(entryID == JUMP_KEY)
					m_DuckAndCover.jumpKey = (byte)i;

				m_DuckAndCover.SaveToRegistry();
				break;
			}
	}
	else
	{
		/**
		* ESCAPE : Set menu inactive and save the configuration.
		***/
		if (controls.Key_Down(VK_ESCAPE))
		{
			VPMENU_mode = VPMENU_Modes::INACTIVE;
			VPMENU_UpdateConfigSettings();
		}

		if ((controls.Key_Down(VK_RETURN) || controls.Key_Down(VK_RSHIFT) || (controls.xButtonsStatus[0x0c])) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			if (entryID == CROUCH_KEY)
			{
				hotkeyCatch = true;
				menuVelocity.x+=2.0f;
			}

			if (entryID == CROUCH_TOGGLE)
			{
				m_DuckAndCover.crouchToggle = !m_DuckAndCover.crouchToggle;
				m_DuckAndCover.SaveToRegistry();
				menuVelocity.x+=2.0f;
			}

			if (entryID == PRONE_KEY)
			{
				hotkeyCatch = true;
				menuVelocity.x+=2.0f;
			}

			if (entryID == PRONE_TOGGLE)
			{
				m_DuckAndCover.proneToggle = !m_DuckAndCover.proneToggle;
				m_DuckAndCover.SaveToRegistry();
				menuVelocity.x+=2.0f;
			}

			if (entryID == JUMP_KEY)
			{
				hotkeyCatch = true;
				menuVelocity.x+=2.0f;
			}

			if (entryID == JUMP_ENABLED)
			{
				m_DuckAndCover.jumpEnabled = !m_DuckAndCover.jumpEnabled;
				m_DuckAndCover.SaveToRegistry();
				menuVelocity.x+=2.0f;
			}

			// start calibration
			if (entryID == DUCKANDCOVER_CALIBRATE)
			{
				VPMENU_mode = VPMENU_Modes::INACTIVE;
				m_DuckAndCover.dfcStatus = DAC_CAL_STANDING;
				menuVelocity.x += 3.0f;
			}

			// enable/disable - calibrate if not previously calibrated
			if (entryID == DUCKANDCOVER_MODE)
			{
				if (m_DuckAndCover.dfcStatus == DAC_INACTIVE)
				{
					VPMENU_mode = VPMENU_Modes::INACTIVE;
					m_DuckAndCover.dfcStatus = DAC_CAL_STANDING;
				}
				else if (m_DuckAndCover.dfcStatus == DAC_DISABLED)
				{
					//Already calibrated, so just set to standing again
					m_DuckAndCover.dfcStatus = DAC_STANDING;
				}
				else
				{
					//Already enabled, so disable
					m_DuckAndCover.dfcStatus = DAC_DISABLED;
				}
					
				menuVelocity.x += 3.0f;
			}

			// back to main menu
			if (entryID == BACK_VPMENU)
			{
				VPMENU_mode = VPMENU_Modes::MAINMENU;
				VPMENU_UpdateConfigSettings();
				menuVelocity.x+=2.0f;
			}

			// back to game
			if (entryID == BACK_GAME)
			{
				VPMENU_mode = VPMENU_Modes::INACTIVE;
				VPMENU_UpdateConfigSettings();
			}
		}
	}

	// output menu
	if (hudFont)
	{
		// adjust border
		float borderDrawingHeight = borderTopHeight;
		if ((menuVelocity.y < 1.0f) && (menuVelocity.y > -1.0f))
			borderTopHeight = menuTop+menuEntryHeight*(float)borderSelection;

		// draw border - total width due to shift correction
		D3DRECT rect;
		rect.x1 = (int)0; rect.x2 = (int)viewportWidth; rect.y1 = (int)borderTopHeight; rect.y2 = (int)(borderTopHeight+viewportHeight*0.04f);
		ClearEmptyRect(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255,255,128,128), 2);
		ClearEmptyRect(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255,255,128,128), 2);

		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		menuHelperRect.left = 650;
		menuHelperRect.top = 300;
		DrawTextShadowed(hudFont, hudMainMenu, "Vireio Perception ("APP_VERSION") Settings - Duck-and-Cover\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		rect.x1 = 0; rect.x2 = viewportWidth; rect.y1 = (int)(335*fScaleY); rect.y2 = (int)(340*fScaleY);
		Clear(1, &rect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,128,128), 0, 0);

		menuHelperRect.top += 50;  menuHelperRect.left += 150; float guiQSHeight = (float)menuHelperRect.top * fScaleY;
		char vcString[128];

		switch (m_DuckAndCover.crouchToggle)
		{
		case true:
			DrawTextShadowed(hudFont, hudMainMenu, "Crouch : Toggle", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case false:
			DrawTextShadowed(hudFont, hudMainMenu, "Crouch : Hold", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		}
		menuHelperRect.top += MENU_ITEM_SEPARATION;

		sprintf_s(vcString,"Crouch Key : ");
		std::string stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(m_DuckAndCover.crouchKey));
		if ((hotkeyCatch) && (entryID==CROUCH_KEY))
			stdString = "Crouch Key : >Press the desired key<";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;

		if (!m_DuckAndCover.proneEnabled)
		{
			DrawTextShadowed(hudFont, hudMainMenu, "Prone : Disabled (Use calibrate to enable)", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 64, 64));
		}
		else
		{
			switch (m_DuckAndCover.proneToggle)
			{
			case true:
				DrawTextShadowed(hudFont, hudMainMenu, "Prone : Toggle", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
				break;
			case false:
				DrawTextShadowed(hudFont, hudMainMenu, "Prone : Hold", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
				break;
			}
		}
		menuHelperRect.top += MENU_ITEM_SEPARATION;

		sprintf_s(vcString,"Prone Key : ");
		stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(m_DuckAndCover.proneKey));
		if ((hotkeyCatch) && (entryID==PRONE_KEY))
			stdString = "Prone Key : >Press the desired key<";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;

		if (!m_DuckAndCover.jumpEnabled)
			DrawTextShadowed(hudFont, hudMainMenu, "Jump : Enabled", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		else
			DrawTextShadowed(hudFont, hudMainMenu, "Jump : Disabled", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 64, 64));
		menuHelperRect.top += MENU_ITEM_SEPARATION;

		sprintf_s(vcString,"Jump Key : ");
		stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(m_DuckAndCover.jumpKey));
		if ((hotkeyCatch) && (entryID==JUMP_KEY))
			stdString = "Jump Key : >Press the desired key<";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;

		DrawTextShadowed(hudFont, hudMainMenu, "Calibrate Duck-and-Cover then Enable", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;

		if (m_DuckAndCover.dfcStatus == DAC_DISABLED ||
			m_DuckAndCover.dfcStatus == DAC_INACTIVE)
		{
			DrawTextShadowed(hudFont, hudMainMenu, "Enable Duck-and-Cover Mode", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		else
		{
			DrawTextShadowed(hudFont, hudMainMenu, "Disable Duck-and-Cover Mode", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		menuHelperRect.top += MENU_ITEM_SEPARATION;

		DrawTextShadowed(hudFont, hudMainMenu, "Back to Main Menu", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to Game", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;

		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		hudMainMenu->End();
	}
}

/**
* configure Comfort Mode.
***/
void D3DProxyDevice::VPMENU_ComfortMode()
{
	#ifdef SHOW_CALLS
		OutputDebugString("called VPMENU_ComfortMode");
	#endif

	enum
	{
		COMFORT_MODE_ENABLED,
		TURN_LEFT,
		TURN_RIGHT,
		YAW_INCREMENT,
		BACK_VPMENU,
		BACK_GAME,
		NUM_MENU_ITEMS
	};

	UINT menuEntryCount = NUM_MENU_ITEMS;

	menuHelperRect.left = 0;
	menuHelperRect.top = 0;

	UINT entryID;
	VPMENU_NewFrame(entryID, menuEntryCount);
	UINT borderSelection = entryID;
	controls.UpdateXInputs();

	if ((hotkeyCatch) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		for (int i = 0; i < 256; i++)
			if (controls.Key_Down(i) && controls.GetKeyName(i)!="-")
			{
				hotkeyCatch = false;
				if(entryID == TURN_LEFT)
					m_comfortModeLeftKey = (byte)i;
				else if(entryID == TURN_RIGHT)
					m_comfortModeRightKey = (byte)i;
				break;
			}
	}
	else
	{
		/**
		* ESCAPE : Set menu inactive and save the configuration.
		***/
		if (controls.Key_Down(VK_ESCAPE))
		{
			VPMENU_mode = VPMENU_Modes::INACTIVE;
			VPMENU_UpdateConfigSettings();
		}

		if ((controls.Key_Down(VK_RETURN) || controls.Key_Down(VK_RSHIFT) || (controls.xButtonsStatus[0x0c])) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
		{
			if (entryID == COMFORT_MODE_ENABLED)
			{
				VRBoostValue[VRboostAxis::ComfortMode] = 1.0f - VRBoostValue[VRboostAxis::ComfortMode];
				//Reset Yaw to avoid complications
				m_comfortModeYaw = 0.0f;
				menuVelocity.x+=2.0f;
			}

			if (entryID == TURN_LEFT || entryID == TURN_RIGHT)
			{
				hotkeyCatch = true;
				menuVelocity.x+=2.0f;
			}

			if (entryID == YAW_INCREMENT)
			{
				if (m_comfortModeYawIncrement == 30.0f)
					m_comfortModeYawIncrement = 45.0f;
				else if (m_comfortModeYawIncrement == 45.0f)
					m_comfortModeYawIncrement = 60.0f;
				else if (m_comfortModeYawIncrement == 60.0f)
					m_comfortModeYawIncrement = 90.0f;
				else if (m_comfortModeYawIncrement == 90.0f)
					m_comfortModeYawIncrement = 30.0f;
				menuVelocity.x+=2.0f;
			}

			// back to main menu
			if (entryID == BACK_VPMENU)
			{
				VPMENU_mode = VPMENU_Modes::MAINMENU;
				VPMENU_UpdateConfigSettings();
				menuVelocity.x+=2.0f;
			}

			// back to game
			if (entryID == BACK_GAME)
			{
				VPMENU_mode = VPMENU_Modes::INACTIVE;
				VPMENU_UpdateConfigSettings();
			}
		}
	}

	// output menu
	if (hudFont)
	{
		// adjust border
		float borderDrawingHeight = borderTopHeight;
		if ((menuVelocity.y < 1.0f) && (menuVelocity.y > -1.0f))
			borderTopHeight = menuTop+menuEntryHeight*(float)borderSelection;

		// draw border - total width due to shift correction
		D3DRECT rect;
		rect.x1 = (int)0; rect.x2 = (int)viewportWidth; rect.y1 = (int)borderTopHeight; rect.y2 = (int)(borderTopHeight+viewportHeight*0.04f);
		ClearEmptyRect(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255,255,128,128), 2);
		ClearEmptyRect(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255,255,128,128), 2);

		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		menuHelperRect.left = 650;
		menuHelperRect.top = 300;
		DrawTextShadowed(hudFont, hudMainMenu, "Vireio Perception ("APP_VERSION") Settings - Comfort Mode\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		rect.x1 = 0; rect.x2 = viewportWidth; rect.y1 = (int)(335*fScaleY); rect.y2 = (int)(340*fScaleY);
		Clear(1, &rect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,128,128), 0, 0);

		menuHelperRect.top += 50;  menuHelperRect.left += 150; float guiQSHeight = (float)menuHelperRect.top * fScaleY;
		char vcString[128];

		if (VRBoostValue[VRboostAxis::ComfortMode] != 0.0f)
		{
			DrawTextShadowed(hudFont, hudMainMenu, "Comfort Mode : Enabled", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		else
		{
			DrawTextShadowed(hudFont, hudMainMenu, "Comfort Mode : Disabled", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		menuHelperRect.top += MENU_ITEM_SEPARATION;

		sprintf_s(vcString,"Turn Left Key : ");
		std::string stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(m_comfortModeLeftKey));
		if ((hotkeyCatch) && (entryID==TURN_LEFT))
			stdString = "Turn Left Key : >Press the desired key<";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;

		sprintf_s(vcString,"Turn Right Key : ");
		stdString = std::string(vcString);
		stdString.append(controls.GetKeyName(m_comfortModeRightKey));
		if ((hotkeyCatch) && (entryID==TURN_RIGHT))
			stdString = "Turn Right Key : >Press the desired key<";
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;

		sprintf_s(vcString,"Yaw Rotation Increment : %.1f", m_comfortModeYawIncrement);
		stdString = std::string(vcString);
		DrawTextShadowed(hudFont, hudMainMenu, (LPCSTR)stdString.c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;

		DrawTextShadowed(hudFont, hudMainMenu, "Back to Main Menu", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to Game", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;

		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		hudMainMenu->End();
	}
}

/**
* VRBoost constant value sub-menu.
***/
void D3DProxyDevice::VPMENU_VRBoostValues()
{
	#ifdef SHOW_CALLS
		OutputDebugString("called VPMENU_VRBoostValues");
	#endif
	UINT menuEntryCount = 14;

	menuHelperRect.left = 0;
	menuHelperRect.top = 0;

	UINT entryID;
	VPMENU_NewFrame(entryID, menuEntryCount);
	UINT borderSelection = entryID;

	/**
	* ESCAPE : Set menu inactive and save the configuration.
	***/
	if (controls.Key_Down(VK_ESCAPE))
	{
		VPMENU_mode = VPMENU_Modes::INACTIVE;
		VPMENU_UpdateConfigSettings();
	}

	if ((controls.Key_Down(VK_RETURN) || controls.Key_Down(VK_RSHIFT) || (controls.xButtonsStatus[0x0c])) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		// back to main menu
		if (entryID == 12)
		{
			VPMENU_mode = VPMENU_Modes::MAINMENU;
			menuVelocity.x+=2.0f;
		}
		// back to game
		if (entryID == 13)
		{
			VPMENU_mode = VPMENU_Modes::INACTIVE;
			VPMENU_UpdateConfigSettings();
		}
	}

	if ((controls.Key_Down(VK_LEFT) || controls.Key_Down('J') || (controls.xInputState.Gamepad.sThumbLX<-8192)) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		// change value
		if ((entryID >= 0) && (entryID <=11))
		{
			if (controls.xInputState.Gamepad.sThumbLX != 0  && !controls.Key_Down(VK_LEFT) && !controls.Key_Down('J'))
				VRBoostValue[24+entryID] += 0.1f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
			else
				VRBoostValue[24+entryID] -= 0.1f;
			menuVelocity.x-=0.1f;
		}
	}

	if ((controls.Key_Down(VK_RIGHT) || controls.Key_Down('L') || (controls.xInputState.Gamepad.sThumbLX>8192)) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		// change value
		if ((entryID >= 0) && (entryID <=11))
		{
			if (controls.xInputState.Gamepad.sThumbLX != 0  && !controls.Key_Down(VK_RIGHT) && !controls.Key_Down('L'))
				VRBoostValue[24+entryID] += 0.1f * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
			else
				VRBoostValue[24+entryID] += 0.1f;
			menuVelocity.x+=0.1f;
		}
	}

	if (controls.Key_Down(VK_BACK) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		// change value
		if ((entryID >= 3) && (entryID <=11))
		{
			VRBoostValue[24+entryID] = 0.0f;
		}
	}
	
	// output menu
	if (hudFont)
	{
		// adjust border
		float borderDrawingHeight = borderTopHeight;
		if ((menuVelocity.y < 1.0f) && (menuVelocity.y > -1.0f))
			borderTopHeight = menuTop+menuEntryHeight*(float)borderSelection;

		// draw border - total width due to shift correction
		D3DRECT rect;
		rect.x1 = (int)0; rect.x2 = (int)viewportWidth; rect.y1 = (int)borderTopHeight; rect.y2 = (int)(borderTopHeight+viewportHeight*0.04f);
		ClearEmptyRect(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255,255,128,128), 2);
		ClearEmptyRect(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255,255,128,128), 2);

		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		menuHelperRect.left = 650;
		menuHelperRect.top = 300;
		DrawTextShadowed(hudFont, hudMainMenu, "Vireio Perception ("APP_VERSION") Settings - VRBoost\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		rect.x1 = 0; rect.x2 = viewportWidth; rect.y1 = (int)(335*fScaleY); rect.y2 = (int)(340*fScaleY);
		Clear(1, &rect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,128,128), 0, 0);

		menuHelperRect.top += 50;  menuHelperRect.left += 150; float guiQSHeight = (float)menuHelperRect.top * fScaleY;
		char vcString[128];
		sprintf_s(vcString,"World FOV : %g", RoundVireioValue(VRBoostValue[24]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Player FOV : %g", RoundVireioValue(VRBoostValue[25]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Far Plane FOV : %g", RoundVireioValue(VRBoostValue[26]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Camera Translate X : %g", RoundVireioValue(VRBoostValue[27]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Camera Translate Y : %g", RoundVireioValue(VRBoostValue[28]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Camera Translate Z : %g", RoundVireioValue(VRBoostValue[29]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Camera Distance : %g", RoundVireioValue(VRBoostValue[30]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Camera Zoom : %g", RoundVireioValue(VRBoostValue[31]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Camera Horizon Adjustment : %g", RoundVireioValue(VRBoostValue[32]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Constant Value 1 : %g", RoundVireioValue(VRBoostValue[33]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Constant Value 2 : %g", RoundVireioValue(VRBoostValue[34]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		sprintf_s(vcString,"Constant Value 2 : %g", RoundVireioValue(VRBoostValue[35]));
		DrawTextShadowed(hudFont, hudMainMenu, vcString, -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to Main Menu", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += MENU_ITEM_SEPARATION;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to Game", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;

		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		hudMainMenu->End();
	}
}

/**
* VP menu border velocity updated here
* Arrow up/down need to be done via call from Present().
***/
void D3DProxyDevice::VPMENU_UpdateBorder()
{
	#ifdef SHOW_CALLS
		OutputDebugString("called VPMENU_UpdateBorder");
	#endif

	// handle controls 
	if (m_deviceBehavior.whenToHandleHeadTracking == DeviceBehavior::PRESENT)
		HandleTracking();

	// draw 
	if (m_deviceBehavior.whenToRenderVPMENU == DeviceBehavior::PRESENT)
	{
		if ((VPMENU_mode>=VPMENU_Modes::MAINMENU) && (VPMENU_mode<VPMENU_Modes::VPMENU_ENUM_RANGE))
			VPMENU();
		else
			VPMENU_AdditionalOutput();
	}


	//If this is enabled, then draw an apostrophe in the top left corner of the screen at all times
	//this results in obs only picking up the left eye's texture for some reason (total hack, but some users make use of this for streaming
	//using OBS
	if (userConfig.obsStreamHack)
	{
		LPD3DXSPRITE hackSprite = NULL;
		D3DXCreateSprite(this, &hackSprite);
		if (hudFont && hackSprite)
		{
			hackSprite->Begin(D3DXSPRITE_ALPHABLEND);
			D3DXMATRIX matScale;
			D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
			hackSprite->SetTransform(&matScale);			
			menuHelperRect.left = 0;
			menuHelperRect.top = 0;
			menuHelperRect.right = 50;
			menuHelperRect.bottom = 50;
			char buffer[4];
			sprintf_s(buffer, "'");
			hudFont->DrawText(hackSprite, buffer, -1, &menuHelperRect, DT_LEFT, D3DCOLOR_ARGB(255, 255, 0, 0));
			D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
			hackSprite->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
			hackSprite->End();
			hackSprite->Release();
			hackSprite = NULL;
		}		
	}

	// first, calculate a time scale to adjust the menu speed for the frame speed of the game
	float timeStamp;
	timeStamp = (float)GetTickCount()/1000.0f;
	menuSeconds = timeStamp-menuTime;
	menuTime = timeStamp;
	// Speed up menu - makes an incredible difference!
	float timeScale = (float)menuSeconds*90;

	// menu velocity present ? in case calculate diminution of the velocity
	if (menuVelocity != D3DXVECTOR2(0.0f, 0.0f))
	{
		float diminution = 0.05f;
		diminution *= timeScale;
		if (diminution > 1.0f) diminution = 1.0f;
		menuVelocity*=1.0f-diminution;

		// set velocity to zero in case of low velocity
		if ((menuVelocity.y<0.9f) && (menuVelocity.y>-0.9f) &&
			(menuVelocity.x<0.7f) && (menuVelocity.x>-0.7f))
			menuVelocity = D3DXVECTOR2(0.0f, 0.0f);
	}

	// vp menu active ? handle up/down controls
	if (VPMENU_mode != VPMENU_Modes::INACTIVE)
	{
		int viewportHeight = stereoView->viewport.Height;

		float fScaleY = ((float)viewportHeight / (float)1080.0f);
		if ((controls.Key_Down(VK_UP) || controls.Key_Down('I') || (controls.xInputState.Gamepad.sThumbLY>8192)) && (menuVelocity.y==0.0f))
			menuVelocity.y=-2.7f;
		if ((controls.Key_Down(VK_DOWN) || controls.Key_Down('K') || (controls.xInputState.Gamepad.sThumbLY<-8192)) && (menuVelocity.y==0.0f))
			menuVelocity.y=2.7f;
		if ((controls.Key_Down(VK_PRIOR) || controls.Key_Down('U')) && (menuVelocity.y==0.0f))
			menuVelocity.y=-15.0f;
		if ((controls.Key_Down(VK_NEXT) ||controls.Key_Down('O')) && (menuVelocity.y==0.0f))
			menuVelocity.y=15.0f;
		borderTopHeight += (menuVelocity.y+menuAttraction.y)*fScaleY*timeScale;
	}
}

/**
* Updates the current config based on the current device settings.
***/
void D3DProxyDevice::VPMENU_UpdateConfigSettings()
{
	#ifdef SHOW_CALLS
		OutputDebugString("called VPMENU_UpdateConfigSettings");
	#endif
	ProxyHelper* helper = new ProxyHelper();

	config.roll_multiplier = tracker->multiplierRoll;
	config.yaw_multiplier = tracker->multiplierYaw;
	config.pitch_multiplier = tracker->multiplierPitch;
	config.YOffset = stereoView->YOffset;
	config.IPDOffset = stereoView->IPDOffset;
	config.swap_eyes = stereoView->swapEyes;
	config.DistortionScale = stereoView->DistortionScale;
	config.hud3DDepthMode = (int)hud3DDepthMode;
	for (int i = 0; i < 4; i++)
	{
		config.hud3DDepthPresets[i] = hud3DDepthPresets[i];
		config.hudDistancePresets[i] = hudDistancePresets[i];
		config.hudHotkeys[i] = hudHotkeys[i];
	}
	config.hudHotkeys[4] = hudHotkeys[4];

	config.gui3DDepthMode = (int)gui3DDepthMode;
	for (int i = 0; i < 4; i++)
	{
		config.gui3DDepthPresets[i] = gui3DDepthPresets[i];
		config.guiSquishPresets[i] = guiSquishPresets[i];
		config.guiHotkeys[i] = guiHotkeys[i];
	}
	config.guiHotkeys[4] = guiHotkeys[4];

	config.VRBoostResetHotkey = toggleVRBoostHotkey;
	config.EdgePeekHotkey = edgePeekHotkey;
	config.WorldFOV = VRBoostValue[VRboostAxis::WorldFOV];
	config.PlayerFOV = VRBoostValue[VRboostAxis::PlayerFOV];
	config.FarPlaneFOV = VRBoostValue[VRboostAxis::FarPlaneFOV];
	config.CameraTranslateX = VRBoostValue[VRboostAxis::CameraTranslateX];
	config.CameraTranslateY = VRBoostValue[VRboostAxis::CameraTranslateY];
	config.CameraTranslateZ = VRBoostValue[VRboostAxis::CameraTranslateZ];
	config.CameraDistance = VRBoostValue[VRboostAxis::CameraDistance];
	config.CameraZoom = VRBoostValue[VRboostAxis::CameraZoom];
	config.CameraHorizonAdjustment = VRBoostValue[VRboostAxis::CameraHorizonAdjustment];
	config.ConstantValue1 = VRBoostValue[VRboostAxis::ConstantValue1];
	config.ConstantValue2 = VRBoostValue[VRboostAxis::ConstantValue2];
	config.ConstantValue3 = VRBoostValue[VRboostAxis::ConstantValue3];

	m_spShaderViewAdjustment->Save(config);
	helper->SaveConfig(config);
	delete helper;
}

/**
* Updates all device settings read from the current config.
***/
void D3DProxyDevice::VPMENU_UpdateDeviceSettings()
{
	#ifdef SHOW_CALLS
		OutputDebugString("called VPMENU_UpdateDeviceSettings");
	#endif
	m_spShaderViewAdjustment->Load(config);
	stereoView->DistortionScale = config.DistortionScale;

	// HUD
	for (int i = 0; i < 4; i++)
	{
		hud3DDepthPresets[i] = config.hud3DDepthPresets[i];
		hudDistancePresets[i] = config.hudDistancePresets[i];
		hudHotkeys[i] = config.hudHotkeys[i];
	}
	hudHotkeys[4] = config.hudHotkeys[4];
	ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)config.hud3DDepthMode);

	// GUI
	for (int i = 0; i < 4; i++)
	{
		gui3DDepthPresets[i] = config.gui3DDepthPresets[i];
		guiSquishPresets[i] = config.guiSquishPresets[i];
		guiHotkeys[i] = config.guiHotkeys[i];
	}
	guiHotkeys[4] = config.guiHotkeys[4];
	ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)config.gui3DDepthMode);

	//Disconnected Screen Mode
	edgePeekHotkey = config.EdgePeekHotkey;
	// VRBoost
	toggleVRBoostHotkey = config.VRBoostResetHotkey;
	VRBoostValue[VRboostAxis::WorldFOV] = config.WorldFOV;
	VRBoostValue[VRboostAxis::PlayerFOV] = config.PlayerFOV;
	VRBoostValue[VRboostAxis::FarPlaneFOV] = config.FarPlaneFOV;
	VRBoostValue[VRboostAxis::CameraTranslateX] = config.CameraTranslateX;
	VRBoostValue[VRboostAxis::CameraTranslateY] = config.CameraTranslateY;
	VRBoostValue[VRboostAxis::CameraTranslateZ] = config.CameraTranslateZ;
	VRBoostValue[VRboostAxis::CameraDistance] = config.CameraDistance;
	VRBoostValue[VRboostAxis::CameraZoom] = config.CameraZoom;
	VRBoostValue[VRboostAxis::CameraHorizonAdjustment] = config.CameraHorizonAdjustment;
	VRBoostValue[VRboostAxis::ConstantValue1] = config.ConstantValue1;
	VRBoostValue[VRboostAxis::ConstantValue2] = config.ConstantValue2;
	VRBoostValue[VRboostAxis::ConstantValue3] = config.ConstantValue3;

	// set behavior accordingly to game type
	int gameType = config.game_type;
	if (gameType>10000) gameType-=10000;
	switch(gameType)
	{
	case D3DProxyDevice::FIXED:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::BEGIN_SCENE;
		m_deviceBehavior.whenToRenderVPMENU = DeviceBehavior::WhenToDo::PRESENT;
		break;
	case D3DProxyDevice::SOURCE:
	case D3DProxyDevice::SOURCE_L4D:
	case D3DProxyDevice::SOURCE_ESTER:
	case D3DProxyDevice::SOURCE_STANLEY:
	case D3DProxyDevice::SOURCE_ZENO:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::END_SCENE;
		m_deviceBehavior.whenToRenderVPMENU = DeviceBehavior::WhenToDo::END_SCENE;
		break;
	case D3DProxyDevice::SOURCE_HL2:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::BEGIN_SCENE;
		m_deviceBehavior.whenToRenderVPMENU = DeviceBehavior::WhenToDo::END_SCENE;
		break;
	case D3DProxyDevice::UNREAL:
	case D3DProxyDevice::UNREAL_MIRROR:
	case D3DProxyDevice::UNREAL_UT3:
	case D3DProxyDevice::UNREAL_BIOSHOCK:
	case D3DProxyDevice::UNREAL_BIOSHOCK2:
	case D3DProxyDevice::UNREAL_BORDERLANDS:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::END_SCENE;
		m_deviceBehavior.whenToRenderVPMENU = DeviceBehavior::WhenToDo::END_SCENE;
		break;
	case D3DProxyDevice::UNREAL_BETRAYER:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::BEGIN_SCENE;
		m_deviceBehavior.whenToRenderVPMENU = DeviceBehavior::WhenToDo::END_SCENE;
		break;
	case D3DProxyDevice::EGO:
	case D3DProxyDevice::EGO_DIRT:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::END_SCENE;
		m_deviceBehavior.whenToRenderVPMENU = DeviceBehavior::WhenToDo::END_SCENE;
		break;
	case D3DProxyDevice::REALV:
	case D3DProxyDevice::REALV_ARMA:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::BEGIN_SCENE;
		m_deviceBehavior.whenToRenderVPMENU = DeviceBehavior::WhenToDo::END_SCENE;
		break;
	case D3DProxyDevice::UNITY:
	case D3DProxyDevice::UNITY_SLENDER:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::BEGIN_SCENE;
		m_deviceBehavior.whenToRenderVPMENU = DeviceBehavior::WhenToDo::END_SCENE;
		break;
	case D3DProxyDevice::CRYENGINE:
	m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::BEGIN_SCENE;
		m_deviceBehavior.whenToRenderVPMENU = DeviceBehavior::WhenToDo::END_SCENE;
		break;
	case D3DProxyDevice::CRYENGINE_WARHEAD:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::BEGIN_SCENE;
		m_deviceBehavior.whenToRenderVPMENU = DeviceBehavior::WhenToDo::BEGIN_SCENE;
		break;
	case D3DProxyDevice::GAMEBRYO:
	case D3DProxyDevice::GAMEBRYO_SKYRIM:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::BEGIN_SCENE;
		m_deviceBehavior.whenToRenderVPMENU = DeviceBehavior::WhenToDo::END_SCENE;
		break;
	case D3DProxyDevice::LFS:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::BEGIN_SCENE;
		m_deviceBehavior.whenToRenderVPMENU = DeviceBehavior::WhenToDo::END_SCENE;
		break;
	case D3DProxyDevice::CDC:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::END_SCENE;
		m_deviceBehavior.whenToRenderVPMENU = DeviceBehavior::WhenToDo::END_SCENE;
		break;
	case D3DProxyDevice::CHROME:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::BEGIN_SCENE;
		m_deviceBehavior.whenToRenderVPMENU = DeviceBehavior::WhenToDo::END_SCENE;
		break;
	default:
		m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::WhenToDo::BEGIN_SCENE;
		m_deviceBehavior.whenToRenderVPMENU = DeviceBehavior::WhenToDo::PRESENT;
		break;
	}
}

/**
* Additional output when menu is not drawn.
***/
void D3DProxyDevice::VPMENU_AdditionalOutput()
{
	#ifdef SHOW_CALLS
		OutputDebugString("called VPMENU_AdditionalOutput");
	#endif
	// draw vrboost toggle indicator
	if (m_fVRBoostIndicator>0.0f)
	{
		D3DRECT rec;
		rec.x1 = (int)(viewportWidth*(0.5f-(m_fVRBoostIndicator*0.05f))); rec.x2 = (int)(viewportWidth*(0.5f+(m_fVRBoostIndicator*0.05f))); 
		rec.y1 = (int)(viewportHeight*(0.4f-(m_fVRBoostIndicator*0.05f))); rec.y2 = (int)(viewportHeight*(0.4f+(m_fVRBoostIndicator*0.05f)));
		if (m_bVRBoostToggle)
			ClearRect(vireio::RenderPosition::Left, rec, D3DCOLOR_ARGB(255,64,255,64));
		else
			ClearRect(vireio::RenderPosition::Left, rec, D3DCOLOR_ARGB(255,255,128,128));

		// update the indicator float
		m_fVRBoostIndicator-=menuSeconds;
	}

	//Having this here will hijack any other notification - this is intentional
	if (m_DuckAndCover.dfcStatus > DAC_INACTIVE &&
		m_DuckAndCover.dfcStatus < DAC_DISABLED)
	{
		DuckAndCoverCalibrate();
	}

	//Finally, draw any popups if required
	DisplayCurrentPopup();
}


void D3DProxyDevice::DisplayCurrentPopup()
{
	//We don't want to show any notification for the first few seconds (seems to cause an issue in some games!)
	static DWORD initialTick = GetTickCount();
	if ((GetTickCount() - initialTick) < 2000)
		return;

	if ((activePopup.popupType == VPT_NONE && show_fps == FPS_NONE) || 
		VPMENU_mode != VPMENU_Modes::INACTIVE ||
		!userConfig.notifications)
		return;
	
	// output menu
	if (hudFont)
	{
		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		if (activePopup.popupType == VPT_STATS && m_spShaderViewAdjustment->GetStereoType() >= 100)
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
		menuHelperRect.left = 670;
		menuHelperRect.top = 440;
		switch (activePopup.severity)
		{
			case VPS_TOAST:
				{
					//Center on the screen
					format = DT_CENTER;
					popupColour = D3DCOLOR_ARGB(255, 255, 255, 255);
					float FADE_DURATION = 200.0f;
					int fontSize = (activePopup.popupDuration - GetTickCount() > FADE_DURATION) ? 26 : 
						(int)( (25.0f * (activePopup.popupDuration - GetTickCount())) / FADE_DURATION + 1);
					pFont = popupFont[fontSize];
					menuHelperRect.left = 0;
				}
				break;
			case VPS_INFO:
				{
					popupColour = D3DCOLOR_ARGB(255, 128, 255, 128);
					pFont = popupFont[24];
				}
				break;
			case VPS_ERROR:
				{
					popupColour = D3DCOLOR_ARGB(255, 255, 0, 0);
					menuHelperRect.left = 0;
					format = DT_CENTER;
					pFont = errorFont;
				}
				break;
		}

		for (int i = 0; i <= 6; ++i)
		{
			if (strlen(activePopup.line[i]))
				DrawTextShadowed(pFont, hudMainMenu, activePopup.line[i], -1, &menuHelperRect, format, popupColour);
			menuHelperRect.top += MENU_ITEM_SEPARATION;
		}
		
		if (show_fps != FPS_NONE)
		{
			char buffer[256];
			if (show_fps == FPS_COUNT)
				sprintf_s(buffer, "FPS: %.1f", fps);
			else if (show_fps == FPS_TIME)
				sprintf_s(buffer, "Frame Time: %.2f ms", 1000.0f / fps);

			D3DCOLOR colour = D3DCOLOR_ARGB(255, 255, 255, 255);
			if (fps <= 40)
				colour = D3DCOLOR_ARGB(255, 255, 0, 0);
			else if (fps > 74)
				colour = D3DCOLOR_ARGB(255, 0, 255, 0);

			menuHelperRect.top = 800;
			menuHelperRect.left = 0;
			hudFont->DrawText(hudMainMenu, buffer, -1, &menuHelperRect, DT_CENTER, colour);
		}

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;

		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		hudMainMenu->End();
	}
}
