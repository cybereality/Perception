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
#include "Vireio.h"

#ifdef _DEBUG
#include "DxErr.h"
#endif

#include "Version.h"

using namespace VRBoost;
using namespace vireio;
using namespace HotkeyExpressions;

InputBindingRef hotkeyCloseMenu = Key(VK_ESCAPE);
InputBindingRef hotkeyResetToDefault = Key(VK_BACK);
InputBindingRef hotkeyMenuUp = Key(VK_UP) || Key('I');
InputBindingRef hotkeyMenuDown = Key(VK_DOWN) || Key('K');
InputBindingRef hotkeyMenuUpFaster = Key(VK_PRIOR) || Key('U');
InputBindingRef hotkeyMenuDownFaster = Key(VK_NEXT) || Key('O');
InputBindingRef hotkeyMenuSelect = Key(VK_RETURN) || Key(VK_RSHIFT) || Button(0x0c);
InputBindingRef hotkeyAdjustLeft = Key(VK_LEFT) || Key('J');
InputBindingRef hotkeyAdjustRight = Key(VK_RIGHT) || Key('L');

InputBindingRef hotkeyMenuAdjustmentFaster = Key(VK_LSHIFT);
InputBindingRef hotkeyMenuAdjustmentSlower = Key(VK_LCONTROL);
InputBindingRef hotkeyMenuAdjustmentMuchSlower = Key(VK_MENU);

bool D3DProxyDevice::InitVPMENU()
{
	SHOW_CALL("InitVPMENU");
	
	hudFont = NULL;
	menuTime = (float)GetTickCount()/1000.0f;
	ZeroMemory(&m_configBackup, sizeof(m_configBackup));
	screenshot = (int)false;
	m_bForceMouseEmulation = false;
	m_bVRBoostToggle = true;
	m_bPosTrackingToggle = true;
	m_showVRMouse = 0;
	m_fVRBoostIndicator = 0.0f;
	menuIsOpen = false;
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
	inWorldScaleMenu = false;
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


void D3DProxyDevice::VPMENU_Close()
{
	VPMENU_CloseWithoutSaving();
	VPMENU_UpdateConfigSettings();
}

void D3DProxyDevice::VPMENU_CloseWithoutSaving()
{
	hotkeyCatch = false;
	inWorldScaleMenu = false;
	menuIsOpen = false;
}

void D3DProxyDevice::VPMENU_Back()
{
	VPMENU_NavigateTo([=]() {
		VPMENU_MainMenu();
	});
}

void D3DProxyDevice::VPMENU_OpenMainMenu()
{
	borderTopHeight = 0.0f;
	VPMENU_NavigateTo([=]() {
		VPMENU_MainMenu();
	});
}

void D3DProxyDevice::VPMENU_NavigateTo(std::function<void()> menuHandler)
{
	inWorldScaleMenu = false;
	menuIsOpen = true;
	handleCurrentMenu = menuHandler;
	HotkeyCooldown(2.0f);
}

bool D3DProxyDevice::VPMENU_IsOpen()
{
	return menuIsOpen;
}

/**
* VP menu helper to setup new frame.
* @param menuEntryCount [in] The number of menu entries.
***/
void D3DProxyDevice::VPMENU_NewFrame(UINT menuEntryCount)
{
	SHOW_CALL("VPMENU_NewFrame");
	
	menuHelperRect.left = 0;
	menuHelperRect.top = 0;

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
}

int D3DProxyDevice::VPMENU_GetCurrentSelection()
{
	float entry = (borderTopHeight-menuTop+(menuEntryHeight/3.0f))/menuEntryHeight;
	return (int)entry;
}

void D3DProxyDevice::VPMENU_StartDrawing(const char *pageTitle)
{
	// adjust border
	float borderDrawingHeight = borderTopHeight;
	if ((menuVelocity.y < 1.0f) && (menuVelocity.y > -1.0f))
		borderTopHeight = menuTop+menuEntryHeight*(float)VPMENU_GetCurrentSelection();

	// draw border - total width due to shift correction
	D3DRECT rect;
	rect.x1 = (int)0; rect.x2 = (int)viewportWidth; rect.y1 = (int)borderTopHeight; rect.y2 = (int)(borderTopHeight+viewportHeight*0.04f);
	ClearEmptyRect(vireio::RenderPosition::Left, rect, COLOR_MENU_BORDER, 2);
	ClearEmptyRect(vireio::RenderPosition::Right, rect, COLOR_MENU_BORDER, 2);

	hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
	hudMainMenu->SetTransform(&matScale);

	menuHelperRect.left = 650;
	menuHelperRect.top = 300;
	
	std::string pageHeading = retprintf("Vireio Perception (%s) %s\n", APP_VERSION, pageTitle);
	DrawTextShadowed(hudFont, hudMainMenu, pageHeading.c_str(), &menuHelperRect, COLOR_MENU_TEXT);
	rect.x1 = 0; rect.x2 = viewportWidth; rect.y1 = (int)(335*fScaleY); rect.y2 = (int)(340*fScaleY);
	Clear(1, &rect, D3DCLEAR_TARGET, COLOR_MENU_BORDER, 0, 0);
	
	menuHelperRect.top += 50;
	menuHelperRect.left += 150;
}

void D3DProxyDevice::VPMENU_StartDrawing_NonMenu()
{
	hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
	hudMainMenu->SetTransform(&matScale);
}

void D3DProxyDevice::VPMENU_FinishDrawing()
{
	menuHelperRect.left = 0;
	menuHelperRect.top = 0;

	D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
	hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, COLOR_WHITE);
	hudMainMenu->End();
}

bool D3DProxyDevice::VPMENU_Input_Selected()
{
	if(!HotkeysActive())
		return false;
	return hotkeyMenuSelect->IsPressed(controls);
}

bool D3DProxyDevice::VPMENU_Input_Left()
{
	return hotkeyAdjustLeft->IsPressed(controls)
	       || (controls.xInputState.Gamepad.sThumbLX<-8192);
}

bool D3DProxyDevice::VPMENU_Input_Right()
{
	return hotkeyAdjustRight->IsPressed(controls)
	       || (controls.xInputState.Gamepad.sThumbLX>8192);
}

bool D3DProxyDevice::VPMENU_Input_IsAdjustment()
{
	return VPMENU_Input_Left() || VPMENU_Input_Right();
}

float D3DProxyDevice::VPMENU_Input_GetAdjustment()
{
	if (!HotkeysActive())
		return 0;
	
	float speed = VPMENU_Input_SpeedModifier();
	
	if (hotkeyAdjustLeft->IsPressed(controls) && hotkeyAdjustRight->IsPressed(controls))
		return 0;
	else if (hotkeyAdjustRight->IsPressed(controls))
		return speed;
	else if (hotkeyAdjustLeft->IsPressed(controls))
		return -speed;
	else
		return speed * (((float)controls.xInputState.Gamepad.sThumbLX)/32768.0f);
}

float D3DProxyDevice::VPMENU_Input_SpeedModifier()
{
	if(hotkeyMenuAdjustmentSlower->IsPressed(controls))
		return 0.1f;
	else if(hotkeyMenuAdjustmentFaster->IsPressed(controls))
		return 10.0f;
	else if(hotkeyMenuAdjustmentMuchSlower->IsPressed(controls))
		return 0.002f;
	else
		return 1.0;
}

void D3DProxyDevice::VPMENU_BindKey(std::function<void(int)> onBind)
{
	hotkeyCatch = true;
	onBindKey = onBind;
}

/**
* VP menu main method.
***/
void D3DProxyDevice::VPMENU()
{
	SHOW_CALL("VPMENU");
	
	if (!VPMENU_IsOpen())
	{
		VPMENU_AdditionalOutput();
		return;
	}
	
	if (hotkeyCloseMenu->IsPressed(controls))
	{
		VPMENU_Close();
		return;
	}
	if (hotkeyCatch && HotkeysActive())
	{
		for (int i = 0; i < 256; i++)
		{
			if (controls.Key_Down(i) && controls.GetKeyName(i)!="-")
			{
				hotkeyCatch = false;
				onBindKey(i);
			}
		}
	}
	
	if(!hudFont)
		return;
	
	menuConstructionCurrentEntry = 0;
	handleCurrentMenu();
}


/**
* Main Menu method.
***/
void D3DProxyDevice::VPMENU_MainMenu()
{
	SHOW_CALL("VPMENU_MainMenu");
	
	enum
	{
		SHADER_ANALYZER,
		WORLD_SCALE_CALIBRATION,
		CONVERGENCE_ADJUSTMENT,
		HUD_CALIBRATION,
		GUI_CALIBRATION,
		HUD_SCALE,
		GUI_SCALE,
		OVERALL_SETTINGS,
		VRBOOST_VALUES,
		POS_TRACKING_SETTINGS,
		COMFORT_MODE_SETTINGS,
		RESTORE_CONFIGURATION,
		BACK_TO_GAME,
		NUM_MENU_ITEMS
	};
	
	bool includeShaderAnalyzer = (config.game_type > 10000);
	
	UINT menuEntryCount = NUM_MENU_ITEMS;
	if (!includeShaderAnalyzer)
		menuEntryCount--;

	VPMENU_NewFrame(menuEntryCount);

	// output menu
	VPMENU_StartDrawing("Settings");

	if (includeShaderAnalyzer)
	{
		AddNavigationMenuItem("Activate Vireio Shader Analyzer\n",
			[=]() { VPMENU_ShaderSubMenu(); });
	}
	
	AddNavigationMenuItem("World-Scale Calibration\n", [=]() { VPMENU_WorldScale(); });
	AddNavigationMenuItem("Convergence Adjustment\n", [=]() { VPMENU_Convergence(); });
	AddNavigationMenuItem("HUD Calibration\n", [=]() { VPMENU_HUD(); });
	AddNavigationMenuItem("GUI Calibration\n", [=]() { VPMENU_GUI(); });
	
	float hudQSTop = (float)menuHelperRect.top * fScaleY;
	AddMenuItem("HUD Quick Setting : \n", [=]() {
		if (VPMENU_Input_Left() && HotkeysActive())
		{
			if (hud3DDepthMode > HUD_3D_Depth_Modes::HUD_DEFAULT)
				ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)(hud3DDepthMode-1));
			HotkeyCooldown(2.0f);
		}
		if (VPMENU_Input_Right() && HotkeysActive())
		{
			if (hud3DDepthMode < HUD_3D_Depth_Modes::HUD_ENUM_RANGE-1)
				ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)(hud3DDepthMode+1));
			HotkeyCooldown(2.0f);
		}
	});
	float guiQSTop = (float)menuHelperRect.top * fScaleY;
	AddMenuItem("GUI Quick Setting : \n", [=]() {
		if (VPMENU_Input_Left() && HotkeysActive())
		{
			if (gui3DDepthMode > GUI_3D_Depth_Modes::GUI_DEFAULT)
				ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)(gui3DDepthMode-1));
			HotkeyCooldown(2.0f);
		}
		if (VPMENU_Input_Right() && HotkeysActive())
		{
			if (gui3DDepthMode < GUI_3D_Depth_Modes::GUI_ENUM_RANGE-1)
				ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)(gui3DDepthMode+1));
			HotkeyCooldown(2.0f);
		}
	});
	AddNavigationMenuItem("Overall Settings\n", [=]() { VPMENU_Settings(); });
	AddNavigationMenuItem("VRBoost Values\n", [=]() { VPMENU_VRBoostValues(); });
	AddNavigationMenuItem("Position Tracking Configuration\n", [=]() { VPMENU_PosTracking(); });
	AddNavigationMenuItem("Comfort Mode Configuration\n", [=]() { VPMENU_ComfortMode(); });
	AddButtonMenuItem("Restore Configuration\n", [=]() {
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
		HotkeyCooldown(10.0f);
	});
	AddButtonMenuItem("Back to Game\n", [=]() { VPMENU_Close(); });
	
	// draw HUD quick setting rectangles
	D3DRECT rect;
	rect.x1 = (int)(viewportWidth*0.57f); rect.x2 = (int)(viewportWidth*0.61f); rect.y1 = (int)hudQSTop; rect.y2 = (int)(hudQSTop+viewportHeight*0.027f);
	DrawSelection(vireio::RenderPosition::Left, rect, COLOR_QUICK_SETTING, (int)hud3DDepthMode, (int)HUD_3D_Depth_Modes::HUD_ENUM_RANGE);
	DrawSelection(vireio::RenderPosition::Right, rect, COLOR_QUICK_SETTING, (int)hud3DDepthMode, (int)HUD_3D_Depth_Modes::HUD_ENUM_RANGE);

	// draw GUI quick setting rectangles
	rect.x1 = (int)(viewportWidth*0.57f); rect.x2 = (int)(viewportWidth*0.61f); rect.y1 = (int)guiQSTop; rect.y2 = (int)(guiQSTop+viewportHeight*0.027f);
	DrawSelection(vireio::RenderPosition::Left, rect, COLOR_QUICK_SETTING, (int)gui3DDepthMode, (int)GUI_3D_Depth_Modes::GUI_ENUM_RANGE);
	DrawSelection(vireio::RenderPosition::Right, rect, COLOR_QUICK_SETTING, (int)gui3DDepthMode, (int)GUI_3D_Depth_Modes::GUI_ENUM_RANGE);

	VPMENU_FinishDrawing();
}

/**
* World Scale Calibration.
***/
void D3DProxyDevice::VPMENU_WorldScale()
{
	SHOW_CALL("VPMENU_WorldScale");
	inWorldScaleMenu = true;
	
	// base values
	static UINT gameXScaleUnitIndex = 0;

	// ensure that the attraction is set to zero
	// for non-menu-screens like this one
	menuAttraction.x = 0.0f;
	menuAttraction.y = 0.0f;

	// sort the game unit vector
	std::sort (m_gameXScaleUnits.begin(), m_gameXScaleUnits.end());

	// enter ? rshift ? increase gameXScaleUnitIndex
	if (VPMENU_Input_Selected())
	{
		if (controls.Key_Down(VK_LSHIFT))
		{
			if (gameXScaleUnitIndex>0)
				--gameXScaleUnitIndex;
		}
		else
		{
			gameXScaleUnitIndex++;
			
			// game unit index out of range ?
			if ((gameXScaleUnitIndex != 0) && (gameXScaleUnitIndex >= m_gameXScaleUnits.size()))
				gameXScaleUnitIndex = m_gameXScaleUnits.size()-1;
		}
		HotkeyCooldown(2.0f);
	}

	// Left/Right: Decrease/increase world scale
	if (VPMENU_Input_IsAdjustment() && HotkeysActive())
	{
		float separationChange = 0.005f * VPMENU_Input_GetAdjustment();;
		m_spShaderViewAdjustment->ChangeWorldScale(separationChange);
		m_spShaderViewAdjustment->UpdateProjectionMatrices((float)stereoView->viewport.Width/(float)stereoView->viewport.Height);
		HotkeyCooldown(0.7f);
	}
	
	// handle border height (=scrollbar scroll height)
	if (borderTopHeight<-64.0f)
		borderTopHeight = -64.0f;
	if (borderTopHeight>365.0f)
		borderTopHeight = 365.0f;

	// Draw
	// standard hud size, will be scaled later to actual viewport
	int width = VPMENU_PIXEL_WIDTH;
	int height = VPMENU_PIXEL_HEIGHT;

	VPMENU_StartDrawing_NonMenu();

	// arbitrary formular... TODO !! find a more nifty solution
	float BlueLineCenterAsPercentage = m_spShaderViewAdjustment->HMDInfo()->GetLensXCenterOffset() * 0.2f;

	float horWidth = 0.15f;
	int beg = (int)(viewportWidth*(1.0f-horWidth)/2.0) + (int)(BlueLineCenterAsPercentage * viewportWidth * 0.25f);
	int end = (int)(viewportWidth*(0.5f+(horWidth/2.0f))) + (int)(BlueLineCenterAsPercentage * viewportWidth * 0.25f);

	int hashTop = (int)(viewportHeight  * 0.48f);
	int hashBottom = (int)(viewportHeight  * 0.52f);

	RECT rec2 = {(int)(width*0.27f), (int)(height*0.8f),width,height};
	std::string titleText = retprintf("Vireio Perception (%s) Settings - World Scale\n", APP_VERSION);
	DrawTextShadowed(hudFont, hudMainMenu, titleText.c_str(), &rec2, COLOR_MENU_TEXT);

	// draw right line (using BaseDirect3DDevice9, since otherwise we have two lines)
	D3DRECT rec3 = {(int)(viewportWidth/2 + (-BlueLineCenterAsPercentage * viewportWidth * 0.25f))-1, 0,
		(int)(viewportWidth/2 + (-BlueLineCenterAsPercentage * viewportWidth * 0.25f))+1,viewportHeight };
	if (!config.swap_eyes)
		ClearRect(vireio::RenderPosition::Right, rec3, COLOR_BLUE);
	else
		ClearRect(vireio::RenderPosition::Left, rec3, COLOR_BLUE);

	// draw left line (using BaseDirect3DDevice9, since otherwise we have two lines)
	D3DRECT rec4 = {(int)(viewportWidth/2 + (BlueLineCenterAsPercentage * viewportWidth * 0.25f))-1, 0,
		(int)(viewportWidth/2 + (BlueLineCenterAsPercentage * viewportWidth * 0.25f))+1,viewportHeight };
	if (!config.swap_eyes)
		ClearRect(vireio::RenderPosition::Left, rec4, COLOR_RED);
	else
		ClearRect(vireio::RenderPosition::Right, rec4, COLOR_RED);

	// horizontal line
	D3DRECT rec5 = {beg, (viewportHeight /2)-1, end, (viewportHeight /2)+1 };
	if (!config.swap_eyes)
		ClearRect(vireio::RenderPosition::Left, rec5, COLOR_BLUE);
	else
		ClearRect(vireio::RenderPosition::Right, rec5, COLOR_BLUE);

	// hash lines
	int hashNum = 10;
	float hashSpace = horWidth*viewportWidth / (float)hashNum;
	for(int i=0; i<=hashNum; i++) {
		D3DRECT rec5 = {beg+(int)(i*hashSpace)-1, hashTop, beg+(int)(i*hashSpace)+1, hashBottom};
		if (!config.swap_eyes)
			ClearRect(vireio::RenderPosition::Left, rec5, COLOR_HASH_LINE);
		else
			ClearRect(vireio::RenderPosition::Right, rec5, COLOR_HASH_LINE);
	}

	rec2.left = (int)(width*0.35f);
	rec2.top = (int)(height*0.83f);
	DrawTextShadowed(hudFont, hudMainMenu, "World-Scale Calibration", &rec2, COLOR_MENU_TEXT);

	RECT rec10 = {(int)(width*0.40f), (int)(height*0.57f),width,height};
	DrawTextShadowed(hudFont, hudMainMenu, "<- calibrate using Arrow Keys ->", &rec10, COLOR_MENU_TEXT);

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
		DrawTextShadowed(hudFont, hudMainMenu, retprintf("Actual Units %u/%u", gameXScaleUnitIndex, m_gameXScaleUnits.size()), &rec10, COLOR_MENU_TEXT);
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
	float meters = 1 / gameUnit;
	float centimeters = meters * 100.0f;
	float feet = meters * 3.2808399f;
	float inches = feet * 12.0f;
	float gameUnitsToCentimeter =  gameUnit / 100.0f;
	float gameUnitsToFoot = gameUnit / 3.2808399f;
	float gameUnitsToInches = gameUnit / 39.3700787f;
	
	rec10.top = (int)(height*0.6f); rec10.left = (int)(width*0.28f);
	DrawTextShadowed(hudFont, hudMainMenu, retprintf("1 Game Unit = %g Meters", meters).c_str(), &rec10, COLOR_MENU_TEXT);
	rec10.top+=35;
	DrawTextShadowed(hudFont, hudMainMenu, retprintf("1 Game Unit = %g CM", centimeters), &rec10, COLOR_MENU_TEXT);
	rec10.top+=35;
	DrawTextShadowed(hudFont, hudMainMenu, retprintf("1 Game Unit = %g Feet", feet), &rec10, COLOR_MENU_TEXT);
	rec10.top+=35;
	DrawTextShadowed(hudFont, hudMainMenu, retprintf("1 Game Unit = %g In.", inches), &rec10, COLOR_MENU_TEXT);

	RECT rec11 = {(int)(width*0.52f), (int)(height*0.6f),width,height};
	DrawTextShadowed(hudFont, hudMainMenu, retprintf("1 Meter      = %g Game Units", gameUnit), &rec11, COLOR_MENU_TEXT);
	rec11.top+=35;
	DrawTextShadowed(hudFont, hudMainMenu, retprintf("1 CM         = %g Game Units", gameUnitsToCentimeter), &rec11, COLOR_MENU_TEXT);
	rec11.top+=35;
	DrawTextShadowed(hudFont, hudMainMenu, retprintf("1 Foot       = %g Game Units", gameUnitsToFoot), &rec11, COLOR_MENU_TEXT);
	rec11.top+=35;
	DrawTextShadowed(hudFont, hudMainMenu, retprintf("1 Inch       = %g Game Units", gameUnitsToInches), &rec11, COLOR_MENU_TEXT);

	VPMENU_FinishDrawing();

	// draw description text box
	VPMENU_StartDrawing_NonMenu();
	RECT rec8 = {620, (int)(borderTopHeight), 1300, 400};
	DrawTextShadowed(hudFont, hudTextBox,
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
		"further for comfort and game unit accuracy.\n",
		&rec8, COLOR_MENU_TEXT);
	D3DXVECTOR3 vPos(0.0f, 0.0f, 0.0f);
	hudTextBox->Draw(NULL, &rec8, NULL, &vPos, COLOR_WHITE);

	// draw description box scroll bar
	float scroll = (429.0f-borderTopHeight-64.0f)/429.0f;
	D3DRECT rec9 = {(int)(1300*fScaleX), 0, (int)(1320*fScaleX), (int)(400*fScaleY)};
	DrawScrollbar(vireio::RenderPosition::Left, rec9, COLOR_QUICK_SETTING, scroll, (int)(20*fScaleY));
	DrawScrollbar(vireio::RenderPosition::Right, rec9, COLOR_QUICK_SETTING, scroll, (int)(20*fScaleY));

	hudTextBox->End();
}

/**
* Convergence Adjustment.
***/
void D3DProxyDevice::VPMENU_Convergence()
{
	SHOW_CALL("VPMENU_Convergence");
	
	// ensure that the attraction is set to zero
	// for non-menu-screens like this one
	menuAttraction.x = 0.0f;
	menuAttraction.y = 0.0f;

	// Left/Right: Decrease/Increase convergence (hold CTRL to lower speed, SHIFT to speed up)
	if (VPMENU_Input_IsAdjustment() && HotkeysActive())
	{
		float convergenceChange = 0.05f * VPMENU_Input_GetAdjustment();
		m_spShaderViewAdjustment->ChangeConvergence(convergenceChange);
		m_spShaderViewAdjustment->UpdateProjectionMatrices((float)stereoView->viewport.Width/(float)stereoView->viewport.Height);
		HotkeyCooldown(0.7f);
	}
	

	// handle border height (=scrollbar scroll height)
	if (borderTopHeight<-64.0f)
		borderTopHeight = -64.0f;
	if (borderTopHeight>365.0f)
		borderTopHeight = 365.0f;

	// Draw
	// standard hud size, will be scaled later to actual viewport
	int width = VPMENU_PIXEL_WIDTH;
	int height = VPMENU_PIXEL_HEIGHT;

	VPMENU_StartDrawing_NonMenu();

	// arbitrary formular... TODO !! find a more nifty solution
	float BlueLineCenterAsPercentage = m_spShaderViewAdjustment->HMDInfo()->GetLensXCenterOffset() * 0.2f;

	float horWidth = 0.15f;
	int beg = (int)(viewportWidth*(1.0f-horWidth)/2.0) + (int)(BlueLineCenterAsPercentage * viewportWidth * 0.25f);
	int end = (int)(viewportWidth*(0.5f+(horWidth/2.0f))) + (int)(BlueLineCenterAsPercentage * viewportWidth * 0.25f);

	int hashTop = (int)(viewportHeight  * 0.48f);
	int hashBottom = (int)(viewportHeight  * 0.52f);

	RECT rec2 = {(int)(width*0.27f), (int)(height*0.8f),width,height};
	std::string titleText = retprintf("Vireio Perception (%s) Settings - Convergence\n", APP_VERSION);
	DrawTextShadowed(hudFont, hudMainMenu, titleText.c_str(), &rec2, COLOR_MENU_TEXT);

	// draw right line (using BaseDirect3DDevice9, since otherwise we have two lines)
	D3DRECT rec3 = {(int)(viewportWidth/2 + (-BlueLineCenterAsPercentage * viewportWidth * 0.25f))-1, 0,
		(int)(viewportWidth/2 + (-BlueLineCenterAsPercentage * viewportWidth * 0.25f))+1,viewportHeight };
	if (!config.swap_eyes)
		ClearRect(vireio::RenderPosition::Right, rec3, COLOR_BLUE);
	else
		ClearRect(vireio::RenderPosition::Left, rec3, COLOR_BLUE);

	// draw left line (using BaseDirect3DDevice9, since otherwise we have two lines)
	D3DRECT rec4 = {(int)(viewportWidth/2 + (BlueLineCenterAsPercentage * viewportWidth * 0.25f))-1, 0,
		(int)(viewportWidth/2 + (BlueLineCenterAsPercentage * viewportWidth * 0.25f))+1,viewportHeight };
	if (!config.swap_eyes)
		ClearRect(vireio::RenderPosition::Left, rec4, COLOR_BLUE);
	else
		ClearRect(vireio::RenderPosition::Right, rec4, COLOR_BLUE);

	// horizontal line
	D3DRECT rec5 = {beg, (viewportHeight /2)-1, end, (viewportHeight /2)+1 };
	if (!config.swap_eyes)
		ClearRect(vireio::RenderPosition::Left, rec5, COLOR_BLUE);
	else
		ClearRect(vireio::RenderPosition::Right, rec5, COLOR_BLUE);

	// hash lines
	int hashNum = 10;
	float hashSpace = horWidth*viewportWidth / (float)hashNum;
	for(int i=0; i<=hashNum; i++) {
		D3DRECT rec5 = {beg+(int)(i*hashSpace)-1, hashTop, beg+(int)(i*hashSpace)+1, hashBottom};
		if (!config.swap_eyes)
			ClearRect(vireio::RenderPosition::Left, rec5, COLOR_HASH_LINE);
		else
			ClearRect(vireio::RenderPosition::Right, rec5, COLOR_HASH_LINE);
	}

	rec2.left = (int)(width*0.35f);
	rec2.top = (int)(height*0.83f);
	DrawTextShadowed(hudFont, hudMainMenu, "Convergence Adjustment", &rec2, COLOR_MENU_TEXT);

	// output convergence
	RECT rec10 = {(int)(width*0.40f), (int)(height*0.57f),width,height};
	DrawTextShadowed(hudFont, hudMainMenu, "<- calibrate using Arrow Keys ->", &rec10, COLOR_MENU_TEXT);
	// Convergence Screen = X Meters = X Feet
	rec10.top = (int)(height*0.6f); rec10.left = (int)(width*0.385f);
	
	float meters = m_spShaderViewAdjustment->Convergence();
	float centimeters = meters * 100.0f;
	float feet = meters * 3.2808399f;
	float inches = feet * 12.0f;
	
	DrawTextShadowed(hudFont, hudMainMenu, retprintf("Convergence Screen = %g Meters", meters), &rec10, COLOR_MENU_TEXT);
	rec10.top+=35;
	DrawTextShadowed(hudFont, hudMainMenu, retprintf("Convergence Screen = %g CM", centimeters), &rec10, COLOR_MENU_TEXT);
	rec10.top+=35;
	DrawTextShadowed(hudFont, hudMainMenu, retprintf("Convergence Screen = %g Feet", feet), &rec10, COLOR_MENU_TEXT);
	rec10.top+=35;
	DrawTextShadowed(hudFont, hudMainMenu, retprintf("Convergence Screen = %g Inches", inches), &rec10, COLOR_MENU_TEXT);

	VPMENU_FinishDrawing();

	// draw description text box
	VPMENU_StartDrawing_NonMenu();
	RECT rec8 = {620, (int)(borderTopHeight), 1300, 400};
	DrawTextShadowed(hudFont, hudTextBox,
		"Note that the Convergence Screens distance\n"
		"is measured in physical meters and should\n"
		"only be adjusted to match Your personal\n"
		"depth cognition after You calibrated the\n"
		"World Scale accordingly.\n"
		"In the right eye view, walk up as close as\n"
		"possible to a 90 degree vertical object and\n"
		"align the BLUE vertical line with its edge.\n"
		"Good examples include a wall corner, a table\n"
		"corner, a square post, etc.\n",
		&rec8, COLOR_MENU_TEXT);
	D3DXVECTOR3 vPos(0.0f, 0.0f, 0.0f);
	hudTextBox->Draw(NULL, &rec8, NULL, &vPos, COLOR_WHITE);

	// draw description box scroll bar
	float scroll = (429.0f-borderTopHeight-64.0f)/429.0f;
	D3DRECT rec9 = {(int)(1300*fScaleX), 0, (int)(1320*fScaleX), (int)(400*fScaleY)};
	DrawScrollbar(vireio::RenderPosition::Left, rec9, COLOR_QUICK_SETTING, scroll, (int)(20*fScaleY));
	DrawScrollbar(vireio::RenderPosition::Right, rec9, COLOR_QUICK_SETTING, scroll, (int)(20*fScaleY));

	hudTextBox->End();
}

/**
* HUD Calibration.
***/
void D3DProxyDevice::VPMENU_HUD()
{
	SHOW_CALL("VPMENU_HUD");
	
	enum
	{
		HUD_MODE = 0,
		HUD_DISTANCE = 1,
		HUD_3DDEPTH = 2,
		HUD_HOTKEYS_START = 3,
		HUD_HOTKEYS_END = 7,
		BACK_VPMENU = 8,
		BACK_GAME = 9,
		NUM_MENU_ITEMS = 10
	};
	
	VPMENU_NewFrame(NUM_MENU_ITEMS);
	
	// output menu
	VPMENU_StartDrawing("Settings - HUD");

	float hudQSHeight = (float)menuHelperRect.top * fScaleY;
	std::string depthModeDescription = "";
	switch (hud3DDepthMode)
	{
		case D3DProxyDevice::HUD_DEFAULT: depthModeDescription = "Default"; break;
		case D3DProxyDevice::HUD_SMALL:   depthModeDescription = "Small"; break;
		case D3DProxyDevice::HUD_LARGE:   depthModeDescription = "Large"; break;
		case D3DProxyDevice::HUD_FULL:    depthModeDescription = "Full"; break;
	}
	
	// change hud scale
	AddMenuItem(retprintf("HUD : %s", depthModeDescription.c_str()), [=]()
	{
		if (VPMENU_Input_Left() && HotkeysActive())
		{
			if (hud3DDepthMode > HUD_3D_Depth_Modes::HUD_DEFAULT)
				ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)(hud3DDepthMode-1));
			HotkeyCooldown(2.0f);
		}
		if (VPMENU_Input_Right() && HotkeysActive())
		{
			if (hud3DDepthMode < HUD_3D_Depth_Modes::HUD_ENUM_RANGE-1)
				ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)(hud3DDepthMode+1));
			HotkeyCooldown(2.0f);
		}
	});
	
	AddMenuItem(retprintf("HUD Distance : %g", RoundVireioValue(hudDistancePresets[(int)hud3DDepthMode])), [=]()
	{
		if (VPMENU_Input_IsAdjustment() && HotkeysActive())
		{
			hudDistancePresets[(int)hud3DDepthMode] += 0.01f * VPMENU_Input_GetAdjustment();
			ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)hud3DDepthMode);
			HotkeyCooldown(0.7f);
		}
	});
	AddMenuItem(retprintf("HUD's 3D Depth : %g", RoundVireioValue(hud3DDepthPresets[(int)hud3DDepthMode])), [=]()
	{
		if (VPMENU_Input_IsAdjustment() && HotkeysActive())
		{
			hud3DDepthPresets[(int)hud3DDepthMode] += 0.002f * VPMENU_Input_GetAdjustment();
			ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)hud3DDepthMode);
			HotkeyCooldown(0.7f);
		}
	});
	
	AddKeybindMenuItem("Hotkey >Switch<",  &hudHotkeys[0]);
	AddKeybindMenuItem("Hotkey >Default<", &hudHotkeys[1]);
	AddKeybindMenuItem("Hotkey >Small<",   &hudHotkeys[2]);
	AddKeybindMenuItem("Hotkey >Large<",   &hudHotkeys[3]);
	AddKeybindMenuItem("Hotkey >Full<",    &hudHotkeys[4]);
	AddButtonMenuItem("Back to Main Menu", [=]() {
		VPMENU_Back();
		VPMENU_UpdateConfigSettings();
	});
	AddButtonMenuItem("Back to Game", [=]() { VPMENU_Close(); });

	// draw HUD quick setting rectangles
	D3DRECT rect;
	rect.x1 = (int)(viewportWidth*0.52f); rect.x2 = (int)(viewportWidth*0.56f); rect.y1 = (int)hudQSHeight; rect.y2 = (int)(hudQSHeight+viewportHeight*0.027f);
	DrawSelection(vireio::RenderPosition::Left, rect, COLOR_QUICK_SETTING, (int)hud3DDepthMode, (int)HUD_3D_Depth_Modes::HUD_ENUM_RANGE);
	DrawSelection(vireio::RenderPosition::Right, rect, COLOR_QUICK_SETTING, (int)hud3DDepthMode, (int)HUD_3D_Depth_Modes::HUD_ENUM_RANGE);

	VPMENU_FinishDrawing();
}

/**
* GUI Calibration.
***/
void D3DProxyDevice::VPMENU_GUI()
{
	SHOW_CALL("VPMENU_GUI");
	
	enum
	{
		GUI_MODE = 0,
		GUI_DISTANCE = 1,
		GUI_3DDEPTH = 2,
		GUI_HOTKEYS_START = 3,
		GUI_HOTKEYS_END = 7,
		BACK_VPMENU = 8,
		BACK_GAME = 9,
		NUM_MENU_ITEMS = 10
	};
	
	VPMENU_NewFrame(NUM_MENU_ITEMS);

	// output menu
	VPMENU_StartDrawing("Settings - GUI");

	float guiQSTop = (float)menuHelperRect.top * fScaleY;
	
	std::string guiDepthModeDescription;
	switch (gui3DDepthMode)
	{
		case D3DProxyDevice::GUI_DEFAULT: guiDepthModeDescription = "Default"; break;
		case D3DProxyDevice::GUI_SMALL:   guiDepthModeDescription = "Small"; break;
		case D3DProxyDevice::GUI_LARGE:   guiDepthModeDescription = "Large"; break;
		case D3DProxyDevice::GUI_FULL:    guiDepthModeDescription = "Full"; break;
	}
	
	AddMenuItem(retprintf("GUI : %s", guiDepthModeDescription.c_str()), [=]()
	{
		if (VPMENU_Input_Left() && HotkeysActive())
		{
			if (gui3DDepthMode > GUI_3D_Depth_Modes::GUI_DEFAULT)
				ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)(gui3DDepthMode-1));
			HotkeyCooldown(2.0f);
		}
		if (VPMENU_Input_Right() && HotkeysActive())
		{
			if (gui3DDepthMode < GUI_3D_Depth_Modes::GUI_ENUM_RANGE-1)
				ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)(gui3DDepthMode+1));
			HotkeyCooldown(2.0f);
		}
	});
	
	AddMenuItem(retprintf("GUI Size : %g", RoundVireioValue(guiSquishPresets[(int)gui3DDepthMode])), [=]()
	{
		if (VPMENU_Input_IsAdjustment() && HotkeysActive())
		{
			guiSquishPresets[(int)gui3DDepthMode] += 0.01f * VPMENU_Input_GetAdjustment();
			ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)gui3DDepthMode);
			HotkeyCooldown(0.7f);
		}
	});
	
	AddMenuItem(retprintf("GUI's 3D Depth : %g", RoundVireioValue(gui3DDepthPresets[(int)gui3DDepthMode])), [=]()
	{
		if (VPMENU_Input_IsAdjustment() && HotkeysActive())
		{
			gui3DDepthPresets[(int)gui3DDepthMode] += 0.002f * VPMENU_Input_GetAdjustment();
			ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)gui3DDepthMode);
			HotkeyCooldown(0.7f);
		}
	});
	
	AddKeybindMenuItem("Hotkey >Switch<",  &guiHotkeys[0]);
	AddKeybindMenuItem("Hotkey >Default<", &guiHotkeys[1]);
	AddKeybindMenuItem("Hotkey >Small<",   &guiHotkeys[2]);
	AddKeybindMenuItem("Hotkey >Large<",   &guiHotkeys[3]);
	AddKeybindMenuItem("Hotkey >Full<",    &guiHotkeys[4]);
	
	AddButtonMenuItem("Back to Main Menu", [=]() { VPMENU_Back(); });
	AddButtonMenuItem("Back to Game", [=]() { VPMENU_Close(); });

	// draw GUI quick setting rectangles
	D3DRECT rect;
	rect.x1 = (int)(viewportWidth*0.52f); rect.x2 = (int)(viewportWidth*0.56f); rect.y1 = (int)guiQSTop; rect.y2 = (int)(guiQSTop+viewportHeight*0.027f);
	DrawSelection(vireio::RenderPosition::Left, rect, COLOR_QUICK_SETTING, (int)gui3DDepthMode, (int)GUI_3D_Depth_Modes::GUI_ENUM_RANGE);
	DrawSelection(vireio::RenderPosition::Right, rect, COLOR_QUICK_SETTING, (int)gui3DDepthMode, (int)GUI_3D_Depth_Modes::GUI_ENUM_RANGE);

	VPMENU_FinishDrawing();
}

/**
* Settings.
***/
void D3DProxyDevice::VPMENU_Settings()
{
	SHOW_CALL("VPMENU_Settings");

	//Use enumeration for menu items to avoid confusion
	enum
	{
		SWAP_EYES,
		IPD_OFFSET,
		Y_OFFSET,
		DISTORTION_SCALE,
		//STEREO_SCREENSHOTS,
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

	VPMENU_NewFrame(NUM_MENU_ITEMS);
	VPMENU_StartDrawing("Settings - General");

	AddButtonMenuItem(retprintf("Swap Eyes : %s", stereoView->swapEyes ? "True" : "False"), [=]()
	{
		stereoView->swapEyes = !stereoView->swapEyes;
	});
	AddAdjustmentMenuItem("IPD-Offset : %1.3f", &this->stereoView->IPDOffset,
		0.0f, 0.001f, [=]()
	{
		clamp(&this->stereoView->IPDOffset, -0.1f, 0.1f);
		this->stereoView->PostReset();
	});
	AddAdjustmentMenuItem("Y-Offset : %1.3f", &this->stereoView->YOffset,
		0.0f, 0.001f, [=]()
	{
		clamp(&this->stereoView->YOffset, -0.1f, 0.1f);
		this->stereoView->PostReset();
	});
	AddAdjustmentMenuItem("Distortion Scale : %g", &this->stereoView->DistortionScale,
		0.0f, 0.01f, [=]()
	{
		this->stereoView->PostReset();
	});
	/*
	AddButtonMenuItem("Stereo Screenshots", [=]() {
		// render 3 frames to get screenshots without menu
		screenshot = 3;
		VPMENU_CloseWithoutSaving();
	});
	*/
	AddAdjustmentMenuItem("Yaw multiplier : %g", &tracker->multiplierYaw, DEFAULT_YAW_MULTIPLIER, 0.05f);
	AddAdjustmentMenuItem("Pitch multiplier : %g", &tracker->multiplierPitch, DEFAULT_PITCH_MULTIPLIER, 0.05f);
	AddAdjustmentMenuItem("Roll multiplier : %g", &tracker->multiplierRoll, 1.0f, 0.05f);
	
	AddButtonMenuItem("Reset Multipliers", [=]()
	{
		tracker->multiplierYaw = DEFAULT_YAW_MULTIPLIER;
		tracker->multiplierPitch = DEFAULT_PITCH_MULTIPLIER;
		tracker->multiplierRoll = 1.0f;
	});
	
	std::string rollImplDescription = "?";
	switch (m_spShaderViewAdjustment->RollImpl())
	{
		case 0: rollImplDescription = "Not Enabled"; break;
		case 1: rollImplDescription = "Matrix Translation"; break;
		case 2: rollImplDescription = "Pixel Shader"; break;
	}
	AddButtonMenuItem(retprintf("Roll : %s", rollImplDescription.c_str()), [=]()
	{
		config.rollImpl = (config.rollImpl+1) % 3;
		m_spShaderViewAdjustment->SetRollImpl(config.rollImpl);
	});
	
	AddMenuItem(retprintf("Force Mouse Emulation HT : %s",
		m_bForceMouseEmulation?"True":"False"), [=]()
	{
		if (VPMENU_Input_Selected() && HotkeysActive())
		{
			m_bForceMouseEmulation = !m_bForceMouseEmulation;

			if ((m_bForceMouseEmulation) && (tracker->getStatus() >= MTS_OK) && (!m_bSurpressHeadtracking))
				tracker->setMouseEmulation(true);

			if ((!m_bForceMouseEmulation) && (hmVRboost) && (m_VRboostRulesPresent)  && (tracker->getStatus() >= MTS_OK))
				tracker->setMouseEmulation(false);

			HotkeyCooldown(4.0f);
		}
		if (VPMENU_Input_Left() && HotkeysActive())
		{
			m_bForceMouseEmulation = false;

			if ((hmVRboost) && (m_VRboostRulesPresent) && (tracker->getStatus() >= MTS_OK))
				tracker->setMouseEmulation(false);

			HotkeyCooldown(2.0f);
		}
		if (VPMENU_Input_Right() && HotkeysActive())
		{
			if(tracker->getStatus() >= MTS_OK)
			{
				tracker->setMouseEmulation(true);
				m_bForceMouseEmulation = true;
			}

			HotkeyCooldown(2.0f);
		}
	});
	
	AddButtonMenuItem(retprintf("Toggle VRBoost : %s",
		m_bVRBoostToggle ? "On" : "Off"),
		m_bVRBoostToggle ? COLOR_MENU_ENABLED : COLOR_MENU_DISABLED,
		[=]()
	{
		if (hmVRboost!=NULL)
		{
			m_pVRboost_ReleaseAllMemoryRules();
			m_bVRBoostToggle = !m_bVRBoostToggle;
			if (tracker->getStatus() >= MTS_OK)
				tracker->resetOrientationAndPosition();
			HotkeyCooldown(2.0f);
		}
	});
	
	AddKeybindMenuItem("Hotkey >Toggle VRBoost<", &toggleVRBoostHotkey);
	AddKeybindMenuItem("Hotkey >Disconnected Screen<", &edgePeekHotkey);
	
	AddButtonMenuItem("Back to Main Menu", [=]() {
		VPMENU_Back();
		VPMENU_UpdateConfigSettings();
	});
	AddButtonMenuItem("Back to Game", [=]() { VPMENU_Close(); });

	VPMENU_FinishDrawing();
}


/**
* Positional Tracking Settings.
***/
void D3DProxyDevice::VPMENU_PosTracking()
{
	SHOW_CALL("VPMENU_PosTracking");

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

	VPMENU_NewFrame(NUM_MENU_ITEMS);
	VPMENU_StartDrawing("Settings - Positional Tracking");

	AddButtonMenuItem(retprintf("Positional Tracking (CTRL + P) : %s",
		m_bPosTrackingToggle ? "On" : "Off"),
		m_bPosTrackingToggle ? COLOR_MENU_ENABLED : COLOR_LIGHTRED,
		[=]()
	{
		m_bPosTrackingToggle = !m_bPosTrackingToggle;

		if (!m_bPosTrackingToggle)
			m_spShaderViewAdjustment->UpdatePosition(0.0f, 0.0f, 0.0f);
	});
	
	AddAdjustmentMenuItem("Position Tracking multiplier : %g", &config.position_multiplier, 1.0f, 0.01f);
	AddAdjustmentMenuItem("Position X-Tracking multiplier : %g", &config.position_x_multiplier, DEFAULT_POS_TRACKING_X_MULT, 0.01f);
	AddAdjustmentMenuItem("Position Y-Tracking multiplier : %g", &config.position_y_multiplier, DEFAULT_POS_TRACKING_Y_MULT, 0.01f);
	AddAdjustmentMenuItem("Position Z-Tracking multiplier : %g", &config.position_z_multiplier, DEFAULT_POS_TRACKING_Z_MULT, 0.01f);
	
	AddButtonMenuItem("Reset HMD Orientation (LSHIFT + R)", [=]() {
		tracker->resetOrientationAndPosition();
	});
	AddNavigationMenuItem("Duck-and-Cover Configuration", [=]() { VPMENU_DuckAndCover(); });
	AddButtonMenuItem("Back to Main Menu", [=]() {
		VPMENU_Back();
		VPMENU_UpdateConfigSettings();
	});
	AddButtonMenuItem("Back to Game", [=]() { VPMENU_Close(); });

	VPMENU_FinishDrawing();
}

/**
* configure DuckAndCover.
***/
void D3DProxyDevice::VPMENU_DuckAndCover()
{
	SHOW_CALL("VPMENU_DuckAndCover");

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
	
	controls.UpdateXInputs();

	VPMENU_NewFrame(NUM_MENU_ITEMS);
	VPMENU_StartDrawing("Settings - Duck-and-Cover");

	std::string crouchToggleDescription = m_DuckAndCover.crouchToggle ? "Toggle" : "Hold";
	AddButtonMenuItem(retprintf("Crouch : %s", crouchToggleDescription.c_str()), [=]() {
		m_DuckAndCover.crouchToggle = !m_DuckAndCover.crouchToggle;
		m_DuckAndCover.SaveToRegistry();
	});

	AddKeybindMenuItem("Crouch Key", &m_DuckAndCover.crouchKey);

	std::string proneToggleDescription =
		!m_DuckAndCover.proneEnabled
		? "Disabled (Use calibrate to enable)" :
			(m_DuckAndCover.proneToggle ? "Toggle" : "Hold");
	AddButtonMenuItem(retprintf("Prone : %s", proneToggleDescription.c_str()),
		m_DuckAndCover.proneEnabled ? COLOR_MENU_TEXT : COLOR_MENU_DISABLED,
		[=]()
	{
		m_DuckAndCover.proneToggle = !m_DuckAndCover.proneToggle;
		m_DuckAndCover.SaveToRegistry();
	});

	AddKeybindMenuItem("Prone Key", &m_DuckAndCover.proneKey);

	AddButtonMenuItem(retprintf("Jump : %s",
		m_DuckAndCover.jumpEnabled ? "Enabled" : "Disabled"),
		m_DuckAndCover.jumpEnabled ? COLOR_MENU_TEXT : COLOR_MENU_DISABLED,
		[=]()
	{
		m_DuckAndCover.jumpEnabled = !m_DuckAndCover.jumpEnabled;
		m_DuckAndCover.SaveToRegistry();
	});

	AddKeybindMenuItem("Jump Key", &m_DuckAndCover.jumpKey);

	AddButtonMenuItem("Calibrate Duck-and-Cover then Enable", [=]() {
		VPMENU_CloseWithoutSaving();
		m_DuckAndCover.dfcStatus = DAC_CAL_STANDING;
	});

	bool dacIsDisabled = (m_DuckAndCover.dfcStatus == DAC_DISABLED ||
		m_DuckAndCover.dfcStatus == DAC_INACTIVE);
	AddButtonMenuItem(retprintf("%s Duck-and-Cover Mode",
		dacIsDisabled?"Enable":"Disable"), [=]()
	{
		if (m_DuckAndCover.dfcStatus == DAC_INACTIVE)
		{
			VPMENU_CloseWithoutSaving();
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
	});

	AddButtonMenuItem("Back to Main Menu", [=]() {
		VPMENU_Back();
		VPMENU_UpdateConfigSettings();
	});
	AddButtonMenuItem("Back to Game", [=]() { VPMENU_Close(); });

	VPMENU_FinishDrawing();
}

/**
* configure Comfort Mode.
***/
void D3DProxyDevice::VPMENU_ComfortMode()
{
	SHOW_CALL("VPMENU_ComfortMode");

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

	controls.UpdateXInputs();
	
	VPMENU_NewFrame(NUM_MENU_ITEMS);
	VPMENU_StartDrawing("Settings - Comfort Mode");

	bool isEnabled = (VRBoostValue[VRboostAxis::ComfortMode] != 0.0f);
	AddButtonMenuItem(retprintf("Comfort Mode : %s", isEnabled?"Enabled":"Disabled"), [=]()
	{
		VRBoostValue[VRboostAxis::ComfortMode] = 1.0f - VRBoostValue[VRboostAxis::ComfortMode];
		//Reset Yaw to avoid complications
		m_comfortModeYaw = 0.0f;
	});

	AddKeybindMenuItem("Turn Left Key", &m_comfortModeLeftKey);
	AddKeybindMenuItem("Turn Right Key", &m_comfortModeRightKey);

	AddButtonMenuItem(retprintf("Yaw Rotation Increment : %.1f", m_comfortModeYawIncrement), [=]()
	{
		if (m_comfortModeYawIncrement == 30.0f)
			m_comfortModeYawIncrement = 45.0f;
		else if (m_comfortModeYawIncrement == 45.0f)
			m_comfortModeYawIncrement = 60.0f;
		else if (m_comfortModeYawIncrement == 60.0f)
			m_comfortModeYawIncrement = 90.0f;
		else if (m_comfortModeYawIncrement == 90.0f)
			m_comfortModeYawIncrement = 30.0f;
		else
			m_comfortModeYawIncrement = 30.0f;
	});

	AddButtonMenuItem("Back to Main Menu", [=]() {
		VPMENU_Back();
		VPMENU_UpdateConfigSettings();
	});
	AddButtonMenuItem("Back to Game", [=]() { VPMENU_Close(); });

	VPMENU_FinishDrawing();
}

/**
* VRBoost constant value sub-menu.
***/
void D3DProxyDevice::VPMENU_VRBoostValues()
{
	SHOW_CALL("VPMENU_VRBoostValues");
	
	enum
	{
		VRBOOSTVALUES_START = 0,
		VRBOOSTVALUES_END = 11,
		BACK_VPMENU = 12,
		BACK_GAME = 13,
		NUM_MENU_ITEMS
	};
	
	VPMENU_NewFrame(NUM_MENU_ITEMS);
	VPMENU_StartDrawing("Settings - VRBoost");

	AddAdjustmentMenuItem("World FOV : %g",          &VRBoostValue[24], 100.0f, 0.5f);
	AddAdjustmentMenuItem("Player FOV : %g",         &VRBoostValue[25], 100.0f, 0.5f);
	AddAdjustmentMenuItem("Far Plane FOV : %g",      &VRBoostValue[26], 100.0f, 0.5f);
	AddAdjustmentMenuItem("Camera Translate X : %g", &VRBoostValue[27], 0.0f, 0.1f);
	AddAdjustmentMenuItem("Camera Translate Y : %g", &VRBoostValue[28], 0.0f, 0.1f);
	AddAdjustmentMenuItem("Camera Translate Z : %g", &VRBoostValue[29], 0.0f, 0.1f);
	AddAdjustmentMenuItem("Camera Distance : %g",    &VRBoostValue[30], 0.0f, 0.1f);
	AddAdjustmentMenuItem("Camera Zoom : %g",        &VRBoostValue[31], 0.0f, 0.1f);
	AddAdjustmentMenuItem("Camera Horizon Adjustment : %g", &VRBoostValue[32], 0.0f, 0.1f);
	AddAdjustmentMenuItem("Constant Value 1 : %g",   &VRBoostValue[33], 0.0f, 0.1f);
	AddAdjustmentMenuItem("Constant Value 2 : %g",   &VRBoostValue[34], 0.0f, 0.1f);
	AddAdjustmentMenuItem("Constant Value 2 : %g",   &VRBoostValue[35], 0.0f, 0.1f);
	AddButtonMenuItem("Back to Main Menu", [=]() { VPMENU_Back(); });
	AddButtonMenuItem("Back to Game", [=]() { VPMENU_Close(); });

	VPMENU_FinishDrawing();
}


/**
* VP menu border velocity updated here
* Arrow up/down need to be done via call from Present().
***/
void D3DProxyDevice::VPMENU_UpdateBorder()
{
	SHOW_CALL("VPMENU_UpdateBorder");

	// handle controls 
	if (m_deviceBehavior.whenToHandleHeadTracking == DeviceBehavior::PRESENT)
		HandleTracking();

	// draw 
	if (m_deviceBehavior.whenToRenderVPMENU == DeviceBehavior::PRESENT)
	{
		VPMENU();
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
			VPMENU_StartDrawing_NonMenu();
			menuHelperRect.left = 0;
			menuHelperRect.top = 0;
			menuHelperRect.right = 50;
			menuHelperRect.bottom = 50;
			hudFont->DrawText(hackSprite, "'", -1, &menuHelperRect, DT_LEFT, COLOR_RED);
			D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
			hackSprite->Draw(NULL, &menuHelperRect, NULL, &vPos, COLOR_WHITE);
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
	if (VPMENU_IsOpen())
	{
		int viewportHeight = stereoView->viewport.Height;

		float fScaleY = ((float)viewportHeight / (float)1080.0f);
		if ((hotkeyMenuUp->IsPressed(controls) || (controls.xInputState.Gamepad.sThumbLY>8192)) && (menuVelocity.y==0.0f))
			menuVelocity.y=-2.7f;
		if ((hotkeyMenuDown->IsPressed(controls) || (controls.xInputState.Gamepad.sThumbLY<-8192)) && (menuVelocity.y==0.0f))
			menuVelocity.y=2.7f;
		if (hotkeyMenuUpFaster->IsPressed(controls) && (menuVelocity.y==0.0f))
			menuVelocity.y=-15.0f;
		if (hotkeyMenuDownFaster->IsPressed(controls) && (menuVelocity.y==0.0f))
			menuVelocity.y=15.0f;
		borderTopHeight += (menuVelocity.y+menuAttraction.y)*fScaleY*timeScale;
	}
}

/**
* Updates the current config based on the current device settings.
***/
void D3DProxyDevice::VPMENU_UpdateConfigSettings()
{
	SHOW_CALL("VPMENU_UpdateConfigSettings");
	
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
	SHOW_CALL("VPMENU_UpdateDeviceSettings");
	
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
	SHOW_CALL("VPMENU_AdditionalOutput");
	
	// draw vrboost toggle indicator
	if (m_fVRBoostIndicator>0.0f)
	{
		D3DRECT rec;
		rec.x1 = (int)(viewportWidth*(0.5f-(m_fVRBoostIndicator*0.05f))); rec.x2 = (int)(viewportWidth*(0.5f+(m_fVRBoostIndicator*0.05f))); 
		rec.y1 = (int)(viewportHeight*(0.4f-(m_fVRBoostIndicator*0.05f))); rec.y2 = (int)(viewportHeight*(0.4f+(m_fVRBoostIndicator*0.05f)));
		if (m_bVRBoostToggle)
			ClearRect(vireio::RenderPosition::Left, rec, COLOR_MENU_ENABLED);
		else
			ClearRect(vireio::RenderPosition::Left, rec, COLOR_LIGHTRED);

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


void D3DProxyDevice::DrawMenuItem(const char *text, D3DCOLOR color)
{
	DrawTextShadowed(hudFont, hudMainMenu, text, &menuHelperRect, color);
	menuHelperRect.top += MENU_ITEM_SEPARATION;
	menuConstructionCurrentEntry++;
}

void D3DProxyDevice::DrawMenuItem(std::string text, D3DCOLOR color)
{
	DrawTextShadowed(hudFont, hudMainMenu, text.c_str(), &menuHelperRect, color);
	menuHelperRect.top += MENU_ITEM_SEPARATION;
	menuConstructionCurrentEntry++;
}

void D3DProxyDevice::AddMenuItem(std::string text, std::function<void()> onHover)
{
	AddMenuItem(text, COLOR_MENU_TEXT, onHover);
}

void D3DProxyDevice::AddMenuItem(std::string text, D3DCOLOR color, std::function<void()> onHover)
{
	int selection = VPMENU_GetCurrentSelection();
	int entryID = menuConstructionCurrentEntry;
	DrawMenuItem(text, color);
	if(entryID == selection)
		onHover();
}

void D3DProxyDevice::AddButtonMenuItem(std::string text, D3DCOLOR color, std::function<void()> onPick)
{
	AddMenuItem(text, color, [=]() {
		if(VPMENU_Input_Selected() && HotkeysActive()) {
			onPick();
			HotkeyCooldown(2.0f);
		}
	});
}

void D3DProxyDevice::AddButtonMenuItem(std::string text, std::function<void()> onPick)
{
	AddButtonMenuItem(text, COLOR_MENU_TEXT, onPick);
}

void D3DProxyDevice::AddNavigationMenuItem(std::string text, std::function<void()> menuHandler)
{
	AddMenuItem(text, COLOR_MENU_TEXT, [=]() {
		if(VPMENU_Input_Selected() && HotkeysActive()) {
			VPMENU_NavigateTo(menuHandler);
		}
	});
}

void D3DProxyDevice::AddKeybindMenuItem(std::string text, byte *binding)
{
	std::string description;
	if (hotkeyCatch && VPMENU_GetCurrentSelection() == menuConstructionCurrentEntry) {
		description = "Press the desired key.";
	} else {
		description = retprintf("%s : %s",
			text.c_str(),
			controls.GetKeyName(*binding).c_str());
	}
	
	AddMenuItem(description, COLOR_MENU_TEXT, [=]() {
		if (hotkeyResetToDefault->IsPressed(controls) && HotkeysActive())
		{
			*binding = 0;
			HotkeyCooldown(2.0f);
		}
		else if (VPMENU_Input_Selected() && HotkeysActive())
		{
			VPMENU_BindKey([=](int key) {
				*binding = (byte)key;
			});
			HotkeyCooldown(2.0f);
		}
	});
}

void D3DProxyDevice::AddAdjustmentMenuItem(const char *formatString, float *value, float defaultValue, float rate, std::function<void()> onChange)
{
	std::string label = retprintf(formatString, RoundVireioValue(*value));
	
	AddMenuItem(label, [=]() {
		if (hotkeyResetToDefault->IsPressed(controls) && HotkeysActive())
		{
			*value = defaultValue;
			onChange();
			HotkeyCooldown(2.0f);
		}
		if (VPMENU_Input_IsAdjustment() && HotkeysActive())
		{
			*value += rate * VPMENU_Input_GetAdjustment();
			onChange();
			HotkeyCooldown(0.7f);
		}
	});
}


/**
* Simple helper to clear a rectangle using the specified color.
* @param renderPosition Left or Right render target to be used.
* @param rect The rectangle in pixel space to be cleared.
* @param color The direct 3d color to be used.
***/
void D3DProxyDevice::ClearRect(vireio::RenderPosition renderPosition, D3DRECT rect, D3DCOLOR color)
{
	SHOW_CALL("ClearRect");
	
	setDrawingSide(renderPosition);
	BaseDirect3DDevice9::Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}

/**
* Simple helper to clear an empty rectangle or border using the specified color.
* @param renderPosition Left or Right render target to be used.
* @param rect The rectangle in pixel space to be cleared.
* @param color The direct 3d color to be used.
* @param bw The border width.
***/
void D3DProxyDevice::ClearEmptyRect(vireio::RenderPosition renderPosition, D3DRECT rect, D3DCOLOR color, int bw)
{
	SHOW_CALL("ClearEmptyRect");
	
	// helper rectangle
	D3DRECT rect0 = D3DRECT(rect);

	setDrawingSide(renderPosition);

	rect0.y2 = rect.y1 + bw;
	BaseDirect3DDevice9::Clear(1, &rect0, D3DCLEAR_TARGET, color, 0, 0);

	rect0.y1 = rect.y2 - bw;
	rect0.y2 = rect.y2;
	BaseDirect3DDevice9::Clear(1, &rect0, D3DCLEAR_TARGET, color, 0, 0);

	rect0.y1 = rect.y1;
	rect0.x2 = rect.x1 + bw;
	BaseDirect3DDevice9::Clear(1, &rect0, D3DCLEAR_TARGET, color, 0, 0);

	rect0.x1 = rect.x2 - bw;
	rect0.x2 = rect.x2;
	BaseDirect3DDevice9::Clear(1, &rect0, D3DCLEAR_TARGET, color, 0, 0);
}

/**
* Draws a simple selection control.
* @param renderPosition Left or Right render target to be used.
* @param rect The rectangle in pixel space to be cleared.
* @param color The direct 3d color to be used.
* @param selectionIndex The index of the currently chosen selection.
* @param selectionRange The range of the selection.
***/
void D3DProxyDevice::DrawSelection(vireio::RenderPosition renderPosition, D3DRECT rect, D3DCOLOR color, int selectionIndex, int selectionRange)
{
	SHOW_CALL("DrawSelection");
	
	// get width of each selection
	float selectionWidth = (rect.x2-rect.x1) / (float)selectionRange;

	// get secondary color
	D3DXCOLOR color2 = D3DXCOLOR(color);
	FLOAT red = color2.r;
	color2.r = color2.g * 0.7f;
	color2.g = red;

	for (int i = 0; i < selectionRange; i++)
	{
		rect.x2 = rect.x1+(int)selectionWidth;
		if (i==selectionIndex)
			ClearRect(renderPosition, rect, color);
		else
			ClearRect(renderPosition, rect, color2);
		rect.x1+=(int)selectionWidth;
	}
}

/**
* Draws a simple selection control.
* @param renderPosition Left or Right render target to be used.
* @param rect The rectangle in pixel space to be cleared.
* @param color The direct 3d color to be used.
* @param selectionIndex The index of the currently chosen selection.
* @param selectionRange The range of the selection.
***/
void D3DProxyDevice::DrawScrollbar(vireio::RenderPosition renderPosition, D3DRECT rect, D3DCOLOR color, float scroll, int scrollbarSize)
{
	SHOW_CALL("DrawScrollbar");
	
	if (scroll<0.0f) scroll=0.0f;
	if (scroll>1.0f) scroll=1.0f;

	// get width of each selection
	int scrollHeight = rect.y2-rect.y1-scrollbarSize;
	scrollHeight = (int)(scrollHeight*scroll);

	// get secondary color
	D3DXCOLOR color2 = D3DXCOLOR(color);
	FLOAT red = color2.r;
	color2.r = color2.g * 0.7f;
	color2.g = red;

	ClearRect(renderPosition, rect, color2);
	rect.y1 += scrollHeight;
	rect.y2 = rect.y1+scrollbarSize;
	ClearRect(renderPosition, rect, color);
}
