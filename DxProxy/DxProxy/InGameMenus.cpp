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

/*** D3DProxyDevice menu functions *******************************************/

bool D3DProxyDevice::InitVPMENU()
{
	SHOW_CALL("InitVPMENU");
	
	hudFont = NULL;
	menuLastUpdateTime = (float)GetTickCount()/1000.0f;
	ZeroMemory(&m_configBackup, sizeof(m_configBackup));
	screenshot = (int)false;
	m_bForceMouseEmulation = false;
	m_bVRBoostToggle = true;
	m_bPosTrackingToggle = true;
	m_showVRMouse = 0;
	m_fVRBoostIndicator = 0.0f;
	menuIsOpen = false;
	hotkeyCooldown = 0.0f;
	hud3DDepthMode = HUD_3D_Depth_Modes::HUD_DEFAULT;
	gui3DDepthMode = GUI_3D_Depth_Modes::GUI_DEFAULT;
	oldHudMode = HUD_3D_Depth_Modes::HUD_DEFAULT;
	oldGuiMode = GUI_3D_Depth_Modes::GUI_DEFAULT;
	ChangeHUD3DDepthMode(HUD_3D_Depth_Modes::HUD_DEFAULT);
	ChangeGUI3DDepthMode(GUI_3D_Depth_Modes::GUI_DEFAULT);

	hotkeyCatch = false;
	inWorldScaleMenu = false;
	toggleVRBoostHotkey = 0;
	edgePeekHotkey = 0;
	
	controls.Reset();
	return true;
}


void D3DProxyDevice::VPMENU_Close()
{
	menuState.onClose();
	VPMENU_CloseWithoutSaving();
	VPMENU_UpdateConfigSettings();
}

void D3DProxyDevice::VPMENU_CloseWithoutSaving()
{
	hotkeyCatch = false;
	inWorldScaleMenu = false;
	menuIsOpen = false;
	
	while(menuStatesStack.size() > 0)
		menuStatesStack.pop();
}

void D3DProxyDevice::VPMENU_Back()
{
	if(menuStatesStack.size() == 0) {
		VPMENU_Close();
	} else {
		menuState.onClose();
		menuState = menuStatesStack.top();
		menuStatesStack.pop();
		inWorldScaleMenu = false;
	}
}

void D3DProxyDevice::VPMENU_OpenMainMenu()
{
	menuState.Reset();
	VPMENU_NavigateTo([=]() {
		VPMENU_MainMenu();
	});
}

void D3DProxyDevice::VPMENU_NavigateTo(std::function<void()> menuHandler)
{
	menuStatesStack.push(menuState);
	menuState.Reset();
	menuState.handleCurrentMenu = menuHandler;
	
	inWorldScaleMenu = false;
	menuIsOpen = true;
	
	HotkeyCooldown(COOLDOWN_SHORT);
}

bool D3DProxyDevice::VPMENU_IsOpen()
{
	return menuIsOpen;
}

/**
* VP menu helper to setup new frame.
***/
MenuBuilder *D3DProxyDevice::VPMENU_NewFrame()
{
	SHOW_CALL("VPMENU_NewFrame");
	
	return new MenuBuilder(this);
}

void D3DProxyDevice::VPMENU_DrawBorder(int top)
{
	// draw border - total width due to shift correction
	D3DRECT rect;
	rect.x1 = (int)0;
	rect.x2 = (int)viewportWidth;
	rect.y1 = (int)top;
	rect.y2 = (int)(top+viewportHeight*0.04f);
	ClearEmptyRect(vireio::RenderPosition::Left, rect, COLOR_MENU_BORDER, 2);
	ClearEmptyRect(vireio::RenderPosition::Right, rect, COLOR_MENU_BORDER, 2);
}

int D3DProxyDevice::VPMENU_GetCurrentSelection()
{
	return menuState.selectedIndex;
}

/**
* 
*/
void D3DProxyDevice::VPMENU_StartDrawing(MenuBuilder *menu, const char *pageTitle)
{
	float borderDrawHeight = menuTop + menuEntryHeight*(menuState.selectedIndex+menuState.animationOffset-menuState.scrollOffset);
	VPMENU_DrawBorder((int)borderDrawHeight);

	hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
	hudMainMenu->SetTransform(&matScale);

	VPMENU_DrawTitle(pageTitle);
	menu->ResetDrawPosition();
}

void D3DProxyDevice::VPMENU_StartDrawing_NonMenu()
{
	hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
	hudMainMenu->SetTransform(&matScale);
}

void D3DProxyDevice::VPMENU_FinishDrawing(MenuBuilder *menu)
{
	RECT copyRect = { 0, 0, VPMENU_PIXEL_WIDTH, VPMENU_PIXEL_HEIGHT };
	D3DXVECTOR3 vPos(0.0f, 0.0f, 0.0f);
	hudMainMenu->Draw(NULL, &copyRect, NULL, &vPos, COLOR_WHITE);
	hudMainMenu->End();
	
	if(menu) {
		VPMENU_UpdateBorder(menu->GetEntryCount());
		delete menu;
	}
}

void D3DProxyDevice::VPMENU_DrawTitle(const char *pageTitle)
{
	RECT titleRect = { 650, 300, VPMENU_PIXEL_WIDTH, VPMENU_PIXEL_HEIGHT };
	std::string pageHeading = retprintf("Vireio Perception (%s) %s\n", APP_VERSION, pageTitle);
	DrawTextShadowed(pageHeading.c_str(), &titleRect);
	
	D3DRECT clearRect;
	clearRect.x1 = 0;
	clearRect.y1 = (int)(335*fScaleY);
	clearRect.x2 = viewportWidth;
	clearRect.y2 = (int)(340*fScaleY);
	Clear(1, &clearRect, D3DCLEAR_TARGET, COLOR_MENU_BORDER, 0, 0);
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
	       || (controls.GetAxis(InputControls::GamepadAxis::LeftStickX)<-MENU_ADJUSTMENT_STICK_DEADZONE);
}

bool D3DProxyDevice::VPMENU_Input_Right()
{
	return hotkeyAdjustRight->IsPressed(controls)
	       || (controls.GetAxis(InputControls::GamepadAxis::LeftStickX)>MENU_ADJUSTMENT_STICK_DEADZONE);
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
		return speed * controls.GetAxis(InputControls::GamepadAxis::LeftStickX);
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

void D3DProxyDevice::VPMENU_BindKey(std::function<void(InputBindingRef)> onBind)
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
	if (hotkeyCatch && hotkeyCooldown == 0.0f)
	{
		std::vector<InputBindingRef> heldInputs = controls.GetHeldInputs();
		if(heldInputs.size() == 0 && bindingHotkeysHeld.size() > 0) {
			// All buttons released, and at least one button was pressed
			hotkeyCatch = false;
			onBindKey(HotkeyExpressions::PackConjunction(bindingHotkeysHeld));
			bindingHotkeysHeld.clear();
			HotkeyCooldown(COOLDOWN_EXTRA_LONG);
		} else {
			for(size_t ii=0; ii<heldInputs.size(); ii++)
			{
				bool isDuplicate = false;
				for(size_t jj=0; jj<bindingHotkeysHeld.size(); jj++) {
					if(heldInputs[ii]->ToString() == bindingHotkeysHeld[jj]->ToString()) {
						isDuplicate = true;
						break;
					}
				}
				
				if(!isDuplicate)
					bindingHotkeysHeld.push_back(heldInputs[ii]);
			}
		}
	}
	
	if(!hudFont)
		return;
	
	menuState.handleCurrentMenu();
}


std::string DepthModeToString(D3DProxyDevice::HUD_3D_Depth_Modes mode)
{
	switch(mode)
	{
		case D3DProxyDevice::HUD_DEFAULT: return "Default";
		case D3DProxyDevice::HUD_SMALL:   return "Small";
		case D3DProxyDevice::HUD_LARGE:   return "Large";
		case D3DProxyDevice::HUD_FULL:    return "Full";
		default: return "???";
	}
}

std::string DepthModeToString(D3DProxyDevice::GUI_3D_Depth_Modes mode)
{
	switch(mode)
	{
		case D3DProxyDevice::GUI_DEFAULT: return "Default";
		case D3DProxyDevice::GUI_SMALL:   return "Small";
		case D3DProxyDevice::GUI_LARGE:   return "Large";
		case D3DProxyDevice::GUI_FULL:    return "Full";
		default: return "???";
	}
}


/**
* Main Menu method.
***/
void D3DProxyDevice::VPMENU_MainMenu()
{
	SHOW_CALL("VPMENU_MainMenu");
	bool includeShaderAnalyzer = (config.game_type > 10000);
	
	MenuBuilder *menu = VPMENU_NewFrame();
	VPMENU_StartDrawing(menu, "Settings");

	if (includeShaderAnalyzer)
	{
		menu->AddNavigation("Shader Analyzer\n",
			[=]() { VPMENU_ShaderSubMenu(); });
	}
	
	menu->AddNavigation("World-Scale Calibration\n", [=]() { VPMENU_WorldScale(); });
	menu->AddNavigation("Convergence Adjustment\n", [=]() { VPMENU_Convergence(); });
	//menu->AddNavigation("HUD Calibration\n", [=]() { VPMENU_HUD(); });
	//menu->AddNavigation("GUI Calibration\n", [=]() { VPMENU_GUI(); });
	
	menu->AddEnumPicker("HUD Settings : %s", (int*)&hud3DDepthMode,
		HUD_3D_Depth_Modes::HUD_ENUM_RANGE, [](int val) {
			return DepthModeToString((HUD_3D_Depth_Modes)val);
		}, [=](int newValue) {
			ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)newValue);
		}, [=]() {
			VPMENU_NavigateTo([=](){ VPMENU_HUD(); });
		});
	menu->AddEnumPicker("GUI Settings : %s", (int*)&gui3DDepthMode,
		GUI_3D_Depth_Modes::GUI_ENUM_RANGE, [](int val) {
			return DepthModeToString((GUI_3D_Depth_Modes)val);
		}, [=](int newValue) {
			ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)newValue);
		}, [=]() {
			VPMENU_NavigateTo([=](){ VPMENU_GUI(); });
		});
	menu->AddNavigation("Overall Settings\n", [=]() { VPMENU_Settings(); });
	menu->AddNavigation("VRBoost Values\n", [=]() { VPMENU_VRBoostValues(); });
	menu->AddNavigation("Position Tracking Configuration\n", [=]() { VPMENU_PosTracking(); });
	menu->AddNavigation("General Hotkeys\n", [=]() { VPMENU_Hotkeys(); });
	menu->AddNavigation("3D Adjustment Hotkeys\n", [=]() { VPMENU_AdjustmentHotkeys(); });
	menu->AddNavigation("Comfort Mode Configuration\n", [=]() { VPMENU_ComfortMode(); });
	
	menu->AddButton("Restore Configuration\n", [=]() {
		// first, backup all strings
		std::string game_exe = config.game_exe;
		std::string shaderRulePath = config.shaderRulePath;
		std::string VRboostPath = config.VRboostPath;
		config = m_configBackup;
		config.game_exe = game_exe;
		config.shaderRulePath = shaderRulePath;
		config.VRboostPath = VRboostPath;
		VPMENU_UpdateDeviceSettings();
		VPMENU_UpdateConfigSettings();
		HotkeyCooldown(COOLDOWN_EXTRA_LONG);
	});
	
	menu->AddButton("Back to Game\n", [=]() { VPMENU_Close(); });
	VPMENU_FinishDrawing(menu);
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
		HotkeyCooldown(COOLDOWN_SHORT);
	}

	// Left/Right: Decrease/increase world scale
	if (VPMENU_Input_IsAdjustment() && HotkeysActive())
	{
		float separationChange = 0.005f * VPMENU_Input_GetAdjustment();;
		m_spShaderViewAdjustment->ChangeWorldScale(separationChange);
		m_spShaderViewAdjustment->UpdateProjectionMatrices((float)stereoView->viewport.Width/(float)stereoView->viewport.Height);
		HotkeyCooldown(COOLDOWN_ONE_FRAME);
	}
	
	// handle border height (=scrollbar scroll height)
	// FIXME: This scrollbar shouldn't be mixed in with menu-scrolling
	if (menuState.borderTopHeight<-64.0f)
		menuState.borderTopHeight = -64.0f;
	if (menuState.borderTopHeight>365.0f)
		menuState.borderTopHeight = 365.0f;

	// Draw
	// standard hud size, will be scaled later to actual viewport
	int width = VPMENU_PIXEL_WIDTH;
	int height = VPMENU_PIXEL_HEIGHT;

	MenuBuilder *menu = VPMENU_NewFrame();
	VPMENU_StartDrawing_NonMenu();

	// arbitrary formular... TODO !! find a more nifty solution
	float BlueLineCenterAsPercentage = m_spShaderViewAdjustment->HMDInfo()->GetLensXCenterOffset() * 0.2f;

	float horWidth = 0.15f;
	int beg = (int)(viewportWidth*(1.0f-horWidth)/2.0) + (int)(BlueLineCenterAsPercentage * viewportWidth * 0.25f);
	int end = (int)(viewportWidth*(0.5f+(horWidth/2.0f))) + (int)(BlueLineCenterAsPercentage * viewportWidth * 0.25f);

	int hashTop = (int)(viewportHeight  * 0.48f);
	int hashBottom = (int)(viewportHeight  * 0.52f);

	std::string titleText = retprintf("Vireio Perception (%s) Settings - World Scale\n", APP_VERSION);
	DrawTextShadowed(width*0.27f, height*0.8f, titleText);

	// draw right line (using BaseDirect3DDevice9, since otherwise we have two lines)
	D3DRECT rec3 = {(int)(viewportWidth/2 + (-BlueLineCenterAsPercentage * viewportWidth * 0.25f))-1, 0,
		(int)(viewportWidth/2 + (-BlueLineCenterAsPercentage * viewportWidth * 0.25f))+1,viewportHeight };
	ClearRect(config.MaybeSwap(vireio::RenderPosition::Right), rec3, COLOR_BLUE);

	// draw left line (using BaseDirect3DDevice9, since otherwise we have two lines)
	D3DRECT rec4 = {(int)(viewportWidth/2 + (BlueLineCenterAsPercentage * viewportWidth * 0.25f))-1, 0,
		(int)(viewportWidth/2 + (BlueLineCenterAsPercentage * viewportWidth * 0.25f))+1,viewportHeight };
	ClearRect(config.MaybeSwap(vireio::RenderPosition::Left), rec4, COLOR_RED);

	// horizontal line
	D3DRECT rec5 = {beg, (viewportHeight /2)-1, end, (viewportHeight /2)+1 };
	ClearRect(config.MaybeSwap(vireio::RenderPosition::Left), rec5, COLOR_BLUE);

	// hash lines
	int hashNum = 10;
	float hashSpace = horWidth*viewportWidth / (float)hashNum;
	for(int i=0; i<=hashNum; i++) {
		D3DRECT rec5 = {beg+(int)(i*hashSpace)-1, hashTop, beg+(int)(i*hashSpace)+1, hashBottom};
		ClearRect(config.MaybeSwap(vireio::RenderPosition::Left), rec5, COLOR_HASH_LINE);
	}

	DrawTextShadowed(width*0.35f, height*0.83f, "World-Scale Calibration");

	DrawTextShadowed(width*0.4f, height*0.57f, "<- calibrate using Arrow Keys ->");

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

		DrawTextShadowed(width*0.45f, height*0.77f, retprintf("Actual Units %u/%u",
			gameXScaleUnitIndex, m_gameXScaleUnits.size()));
	}

	//Column 1:                        Column 2:
	//1 Game Unit = X Meters           1 Meter = X Game Units
	//1 Game Unit = X Centimeters      1 Centimeter = X Game Units
	//1 Game Unit = X Feet             1 Foot = X Game Units
	//1 Game Unit = X Inches           1 Inch = X Game Units
	float meters = 1 / gameUnit;
	float centimeters = meters * 100.0f;
	float feet = meters * 3.2808399f;
	float inches = feet * 12.0f;
	float gameUnitsToCentimeter =  gameUnit / 100.0f;
	float gameUnitsToFoot = gameUnit / 3.2808399f;
	float gameUnitsToInches = gameUnit / 39.3700787f;
	
	float unitsLeft = width*0.28f;
	float unitsTop = height*0.6f;
	float unitsSpacing = 35.0f;
	DrawTextShadowed(unitsLeft, unitsTop,                 retprintf("1 Game Unit = %g Meters", meters).c_str());
	DrawTextShadowed(unitsLeft, unitsTop+unitsSpacing*1,  retprintf("1 Game Unit = %g CM", centimeters));
	DrawTextShadowed(unitsLeft, unitsTop+unitsSpacing*2,  retprintf("1 Game Unit = %g Feet", feet));
	DrawTextShadowed(unitsLeft, unitsTop+unitsSpacing*3,  retprintf("1 Game Unit = %g In.", inches));

	float unitsLeft2 = width*0.52f;
	DrawTextShadowed(unitsLeft2, unitsTop, retprintf("1 Meter      = %g Game Units", gameUnit));
	DrawTextShadowed(unitsLeft2, unitsTop+unitsSpacing*1, retprintf("1 CM         = %g Game Units", gameUnitsToCentimeter));
	DrawTextShadowed(unitsLeft2, unitsTop+unitsSpacing*2, retprintf("1 Foot       = %g Game Units", gameUnitsToFoot));
	DrawTextShadowed(unitsLeft2, unitsTop+unitsSpacing*3, retprintf("1 Inch       = %g Game Units", gameUnitsToInches));

	VPMENU_FinishDrawing(menu);

	// draw description text box
	VPMENU_StartDrawing_NonMenu();
	RECT rec8 = {620, (int)(menuState.borderTopHeight), 1300, 400};
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
	float scroll = (429.0f-menuState.borderTopHeight-64.0f)/429.0f;
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
	
	// Left/Right: Decrease/Increase convergence (hold CTRL to lower speed, SHIFT to speed up)
	if (VPMENU_Input_IsAdjustment() && HotkeysActive())
	{
		float convergenceChange = 0.05f * VPMENU_Input_GetAdjustment();
		m_spShaderViewAdjustment->ChangeConvergence(convergenceChange);
		m_spShaderViewAdjustment->UpdateProjectionMatrices((float)stereoView->viewport.Width/(float)stereoView->viewport.Height);
		HotkeyCooldown(COOLDOWN_ONE_FRAME);
	}
	

	// handle border height (=scrollbar scroll height)
	// FIXME: This scrollbar shouldn't be mixed in with menu-scrolling
	if (menuState.borderTopHeight<-64.0f)
		menuState.borderTopHeight = -64.0f;
	if (menuState.borderTopHeight>365.0f)
		menuState.borderTopHeight = 365.0f;

	// Draw
	// standard hud size, will be scaled later to actual viewport
	int width = VPMENU_PIXEL_WIDTH;
	int height = VPMENU_PIXEL_HEIGHT;

	MenuBuilder *menu = VPMENU_NewFrame();
	VPMENU_StartDrawing_NonMenu();

	// arbitrary formular... TODO !! find a more nifty solution
	float BlueLineCenterAsPercentage = m_spShaderViewAdjustment->HMDInfo()->GetLensXCenterOffset() * 0.2f;

	float horWidth = 0.15f;
	int beg = (int)(viewportWidth*(1.0f-horWidth)/2.0) + (int)(BlueLineCenterAsPercentage * viewportWidth * 0.25f);
	int end = (int)(viewportWidth*(0.5f+(horWidth/2.0f))) + (int)(BlueLineCenterAsPercentage * viewportWidth * 0.25f);

	int hashTop = (int)(viewportHeight  * 0.48f);
	int hashBottom = (int)(viewportHeight  * 0.52f);

	std::string titleText = retprintf("Vireio Perception (%s) Settings - Convergence\n", APP_VERSION);
	DrawTextShadowed(width*0.27f, height*0.8f, titleText.c_str());

	// draw right line (using BaseDirect3DDevice9, since otherwise we have two lines)
	D3DRECT rec3 = {(int)(viewportWidth/2 + (-BlueLineCenterAsPercentage * viewportWidth * 0.25f))-1, 0,
		(int)(viewportWidth/2 + (-BlueLineCenterAsPercentage * viewportWidth * 0.25f))+1,viewportHeight };
	ClearRect(config.MaybeSwap(vireio::RenderPosition::Right), rec3, COLOR_BLUE);

	// draw left line (using BaseDirect3DDevice9, since otherwise we have two lines)
	D3DRECT rec4 = {(int)(viewportWidth/2 + (BlueLineCenterAsPercentage * viewportWidth * 0.25f))-1, 0,
		(int)(viewportWidth/2 + (BlueLineCenterAsPercentage * viewportWidth * 0.25f))+1,viewportHeight };
	ClearRect(config.MaybeSwap(vireio::RenderPosition::Left), rec4, COLOR_BLUE);

	// horizontal line
	D3DRECT rec5 = {beg, (viewportHeight /2)-1, end, (viewportHeight /2)+1 };
	ClearRect(config.MaybeSwap(vireio::RenderPosition::Left), rec5, COLOR_BLUE);

	// hash lines
	int hashNum = 10;
	float hashSpace = horWidth*viewportWidth / (float)hashNum;
	for(int i=0; i<=hashNum; i++) {
		D3DRECT rec5 = {beg+(int)(i*hashSpace)-1, hashTop, beg+(int)(i*hashSpace)+1, hashBottom};
		ClearRect(config.MaybeSwap(vireio::RenderPosition::Left), rec5, COLOR_HASH_LINE);
	}

	DrawTextShadowed(width*0.35f, height*0.83f, "Convergence Adjustment");

	// output convergence
	DrawTextShadowed(width*0.4f, height*0.57f, "<- calibrate using Arrow Keys ->");
	// Convergence Screen = X Meters = X Feet
	float meters = m_spShaderViewAdjustment->Convergence();
	float centimeters = meters * 100.0f;
	float feet = meters * 3.2808399f;
	float inches = feet * 12.0f;
	
	float unitsLeft = width*0.385f;
	float unitsTop = height*0.6f;
	float unitsSpacing = 35.0f;
	DrawTextShadowed(unitsLeft, unitsTop+unitsSpacing*0, retprintf("Convergence Screen = %g Meters", meters));
	DrawTextShadowed(unitsLeft, unitsTop+unitsSpacing*1, retprintf("Convergence Screen = %g CM", centimeters));
	DrawTextShadowed(unitsLeft, unitsTop+unitsSpacing*2, retprintf("Convergence Screen = %g Feet", feet));
	DrawTextShadowed(unitsLeft, unitsTop+unitsSpacing*3, retprintf("Convergence Screen = %g Inches", inches));

	VPMENU_FinishDrawing(menu);

	// draw description text box
	VPMENU_StartDrawing_NonMenu();
	RECT rec8 = {620, (int)(menuState.borderTopHeight), 1300, 400};
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
	float scroll = (429.0f-menuState.borderTopHeight-64.0f)/429.0f;
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
	MenuBuilder *menu = VPMENU_NewFrame();
	VPMENU_StartDrawing(menu, "Settings - HUD");
	menu->OnClose([=]() { VPMENU_UpdateConfigSettings(); });

	menu->AddEnumPicker("HUD : %s", (int*)&hud3DDepthMode,
		HUD_3D_Depth_Modes::HUD_ENUM_RANGE, [](int val) {
			return DepthModeToString((HUD_3D_Depth_Modes)val);
		}, [=](int newValue) {
			ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)newValue);
		});
	
	menu->AddAdjustment("HUD Distance : %g",
		&config.hudDistancePresets[(int)hud3DDepthMode],
		defaultConfig.hudDistancePresets[(int)hud3DDepthMode],
		0.01f, [=]() {
			ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)hud3DDepthMode);
		});
	menu->AddAdjustment("HUD's 3D Depth : %g",
		&config.hud3DDepthPresets[(int)hud3DDepthMode],
		defaultConfig.hud3DDepthPresets[(int)hud3DDepthMode],
		0.002f, [=]() {
			ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)hud3DDepthMode);
		});
	
	menu->AddKeybind("Hotkey >Switch<",  &config.hudSwitchHotkey);
	menu->AddKeybind("Hotkey >Default<", &config.hudHotkeys[0]);
	menu->AddKeybind("Hotkey >Small<",   &config.hudHotkeys[1]);
	menu->AddKeybind("Hotkey >Large<",   &config.hudHotkeys[2]);
	menu->AddKeybind("Hotkey >Full<",    &config.hudHotkeys[3]);

	menu->AddBackButtons();
	VPMENU_FinishDrawing(menu);
}

/**
* GUI Calibration.
***/
void D3DProxyDevice::VPMENU_GUI()
{
	SHOW_CALL("VPMENU_GUI");
	MenuBuilder *menu = VPMENU_NewFrame();
	VPMENU_StartDrawing(menu, "Settings - GUI");
	menu->OnClose([=]() { VPMENU_UpdateConfigSettings(); });

	menu->AddEnumPicker("GUI : %s", (int*)&gui3DDepthMode,
		GUI_3D_Depth_Modes::GUI_ENUM_RANGE, [](int val) {
			return DepthModeToString((GUI_3D_Depth_Modes)val);
		}, [=](int newValue) {
			ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)newValue);
		});
	
	menu->AddAdjustment("GUI Distance : %g",
		&config.guiSquishPresets[(int)gui3DDepthMode],
		defaultConfig.guiSquishPresets[(int)gui3DDepthMode],
		0.01f, [=]() {
			ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)gui3DDepthMode);
		});
	menu->AddAdjustment("GUI's 3D Depth : %g",
		&config.gui3DDepthPresets[(int)gui3DDepthMode],
		defaultConfig.gui3DDepthPresets[(int)gui3DDepthMode],
		0.002f, [=]() {
			ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)gui3DDepthMode);
		});
	
	menu->AddKeybind("Hotkey >Switch<",  &config.guiSwitchHotkey);
	menu->AddKeybind("Hotkey >Default<", &config.guiHotkeys[1]);
	menu->AddKeybind("Hotkey >Small<",   &config.guiHotkeys[2]);
	menu->AddKeybind("Hotkey >Large<",   &config.guiHotkeys[3]);
	menu->AddKeybind("Hotkey >Full<",    &config.guiHotkeys[4]);
	
	menu->AddBackButtons();
	VPMENU_FinishDrawing(menu);
}

/**
* Settings.
***/
void D3DProxyDevice::VPMENU_Settings()
{
	SHOW_CALL("VPMENU_Settings");
	MenuBuilder *menu = VPMENU_NewFrame();
	VPMENU_StartDrawing(menu, "Settings - General");
	menu->OnClose([=]() { VPMENU_UpdateConfigSettings(); });

	menu->AddButton(retprintf("Swap Eyes : %s", stereoView->swapEyes ? "True" : "False"), [=]()
	{
		stereoView->swapEyes = !stereoView->swapEyes;
	});
	menu->AddAdjustment("IPD-Offset : %1.3f", &this->stereoView->IPDOffset,
		0.0f, 0.001f, [=]()
	{
		clamp(&this->stereoView->IPDOffset, -0.1f, 0.1f);
		this->stereoView->PostReset();
	});
	menu->AddAdjustment("Y-Offset : %1.3f", &this->stereoView->YOffset,
		0.0f, 0.001f, [=]()
	{
		clamp(&this->stereoView->YOffset, -0.1f, 0.1f);
		this->stereoView->PostReset();
	});
	menu->AddAdjustment("Distortion Scale : %g", &this->stereoView->DistortionScale,
		0.0f, 0.01f, [=]()
	{
		this->stereoView->PostReset();
	});
	/*
	AddButton("Stereo Screenshots", [=]() {
		// render 3 frames to get screenshots without menu
		screenshot = 3;
		VPMENU_CloseWithoutSaving();
	});
	*/
	menu->AddAdjustment("Yaw multiplier : %g", &tracker->multiplierYaw, DEFAULT_YAW_MULTIPLIER, 0.05f);
	menu->AddAdjustment("Pitch multiplier : %g", &tracker->multiplierPitch, DEFAULT_PITCH_MULTIPLIER, 0.05f);
	menu->AddAdjustment("Roll multiplier : %g", &tracker->multiplierRoll, 1.0f, 0.05f);
	
	menu->AddButton("Reset Multipliers", [=]()
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
	menu->AddButton(retprintf("Roll : %s", rollImplDescription.c_str()), [=]()
	{
		config.rollImpl = (config.rollImpl+1) % 3;
		m_spShaderViewAdjustment->SetRollImpl(config.rollImpl);
	});
	
	menu->AddItem(retprintf("Force Mouse Emulation HT : %s",
		m_bForceMouseEmulation?"True":"False"), [=]()
	{
		if (VPMENU_Input_Selected() && HotkeysActive())
		{
			m_bForceMouseEmulation = !m_bForceMouseEmulation;

			if ((m_bForceMouseEmulation) && (tracker->getStatus() >= MTS_OK) && (!m_bSurpressHeadtracking))
				tracker->setMouseEmulation(true);

			if ((!m_bForceMouseEmulation) && (hmVRboost) && (m_VRboostRulesPresent)  && (tracker->getStatus() >= MTS_OK))
				tracker->setMouseEmulation(false);

			HotkeyCooldown(COOLDOWN_LONG);
		}
		if (VPMENU_Input_Left() && HotkeysActive())
		{
			m_bForceMouseEmulation = false;

			if ((hmVRboost) && (m_VRboostRulesPresent) && (tracker->getStatus() >= MTS_OK))
				tracker->setMouseEmulation(false);

			HotkeyCooldown(COOLDOWN_SHORT);
		}
		if (VPMENU_Input_Right() && HotkeysActive())
		{
			if(tracker->getStatus() >= MTS_OK)
			{
				tracker->setMouseEmulation(true);
				m_bForceMouseEmulation = true;
			}

			HotkeyCooldown(COOLDOWN_SHORT);
		}
	});
	
	menu->AddButton(retprintf("Toggle VRBoost : %s",
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
			HotkeyCooldown(COOLDOWN_SHORT);
		}
	});
	
	menu->AddKeybind("Hotkey >Toggle VRBoost<", &toggleVRBoostHotkey);
	menu->AddKeybind("Hotkey >Disconnected Screen<", &edgePeekHotkey);
	
	menu->AddBackButtons();
	VPMENU_FinishDrawing(menu);
}


/**
* Positional Tracking Settings.
***/
void D3DProxyDevice::VPMENU_PosTracking()
{
	SHOW_CALL("VPMENU_PosTracking");
	MenuBuilder *menu = VPMENU_NewFrame();
	VPMENU_StartDrawing(menu, "Settings - Positional Tracking");
	menu->OnClose([=]() { VPMENU_UpdateConfigSettings(); });

	menu->AddButton(retprintf("Positional Tracking (CTRL + P) : %s",
		m_bPosTrackingToggle ? "On" : "Off"),
		m_bPosTrackingToggle ? COLOR_MENU_ENABLED : COLOR_LIGHTRED,
		[=]()
	{
		m_bPosTrackingToggle = !m_bPosTrackingToggle;

		if (!m_bPosTrackingToggle)
			m_spShaderViewAdjustment->UpdatePosition(0.0f, 0.0f, 0.0f);
	});
	
	menu->AddAdjustment("Position Tracking multiplier : %g", &config.position_multiplier, 1.0f, 0.01f);
	menu->AddAdjustment("Position X-Tracking multiplier : %g", &config.position_x_multiplier, DEFAULT_POS_TRACKING_X_MULT, 0.01f);
	menu->AddAdjustment("Position Y-Tracking multiplier : %g", &config.position_y_multiplier, DEFAULT_POS_TRACKING_Y_MULT, 0.01f);
	menu->AddAdjustment("Position Z-Tracking multiplier : %g", &config.position_z_multiplier, DEFAULT_POS_TRACKING_Z_MULT, 0.01f);
	
	menu->AddButton("Reset HMD Orientation (LSHIFT + R)", [=]() {
		tracker->resetOrientationAndPosition();
	});
	menu->AddNavigation("Duck-and-Cover Configuration", [=]() { VPMENU_DuckAndCover(); });

	menu->AddBackButtons();
	VPMENU_FinishDrawing(menu);
}

/**
* configure DuckAndCover.
***/
void D3DProxyDevice::VPMENU_DuckAndCover()
{
	SHOW_CALL("VPMENU_DuckAndCover");
	controls.UpdateInputs();

	MenuBuilder *menu = VPMENU_NewFrame();
	VPMENU_StartDrawing(menu, "Settings - Duck-and-Cover");
	menu->OnClose([=]() { VPMENU_UpdateConfigSettings(); });

	std::string crouchToggleDescription = m_DuckAndCover.crouchToggle ? "Toggle" : "Hold";
	menu->AddButton(retprintf("Crouch : %s", crouchToggleDescription.c_str()), [=]() {
		m_DuckAndCover.crouchToggle = !m_DuckAndCover.crouchToggle;
		m_DuckAndCover.SaveToRegistry();
	});

	menu->AddGameKeypress("Crouch Key", &m_DuckAndCover.crouchKey);

	std::string proneToggleDescription =
		!m_DuckAndCover.proneEnabled
		? "Disabled (Use calibrate to enable)" :
			(m_DuckAndCover.proneToggle ? "Toggle" : "Hold");
	menu->AddButton(retprintf("Prone : %s", proneToggleDescription.c_str()),
		m_DuckAndCover.proneEnabled ? COLOR_MENU_TEXT : COLOR_MENU_DISABLED,
		[=]()
	{
		m_DuckAndCover.proneToggle = !m_DuckAndCover.proneToggle;
		m_DuckAndCover.SaveToRegistry();
	});

	menu->AddGameKeypress("Prone Key", &m_DuckAndCover.proneKey);

	menu->AddButton(retprintf("Jump : %s",
		m_DuckAndCover.jumpEnabled ? "Enabled" : "Disabled"),
		m_DuckAndCover.jumpEnabled ? COLOR_MENU_TEXT : COLOR_MENU_DISABLED,
		[=]()
	{
		m_DuckAndCover.jumpEnabled = !m_DuckAndCover.jumpEnabled;
		m_DuckAndCover.SaveToRegistry();
	});

	menu->AddGameKeypress("Jump Key", &m_DuckAndCover.jumpKey);

	menu->AddButton("Calibrate Duck-and-Cover then Enable", [=]() {
		VPMENU_CloseWithoutSaving();
		m_DuckAndCover.dfcStatus = DAC_CAL_STANDING;
	});

	bool dacIsDisabled = (m_DuckAndCover.dfcStatus == DAC_DISABLED ||
		m_DuckAndCover.dfcStatus == DAC_INACTIVE);
	menu->AddButton(retprintf("%s Duck-and-Cover Mode",
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

	menu->AddBackButtons();
	VPMENU_FinishDrawing(menu);
}

/**
* configure Comfort Mode.
***/
void D3DProxyDevice::VPMENU_ComfortMode()
{
	SHOW_CALL("VPMENU_ComfortMode");
	controls.UpdateInputs();
	
	MenuBuilder *menu = VPMENU_NewFrame();
	VPMENU_StartDrawing(menu, "Settings - Comfort Mode");
	menu->OnClose([=]() { VPMENU_UpdateConfigSettings(); });

	bool isEnabled = (VRBoostValue[VRboostAxis::ComfortMode] != 0.0f);
	menu->AddButton(retprintf("Comfort Mode : %s", isEnabled?"Enabled":"Disabled"), [=]()
	{
		VRBoostValue[VRboostAxis::ComfortMode] = 1.0f - VRBoostValue[VRboostAxis::ComfortMode];
		//Reset Yaw to avoid complications
		m_comfortModeYaw = 0.0f;
	});

	menu->AddKeybind("Turn Left Key", &config.ComfortModeLeftKey);
	menu->AddKeybind("Turn Right Key", &config.ComfortModeRightKey);

	menu->AddButton(retprintf("Yaw Rotation Increment : %.1f", config.ComfortModeYawIncrement), [=]()
	{
		if (config.ComfortModeYawIncrement == 30.0f)
			config.ComfortModeYawIncrement = 45.0f;
		else if (config.ComfortModeYawIncrement == 45.0f)
			config.ComfortModeYawIncrement = 60.0f;
		else if (config.ComfortModeYawIncrement == 60.0f)
			config.ComfortModeYawIncrement = 90.0f;
		else if (config.ComfortModeYawIncrement == 90.0f)
			config.ComfortModeYawIncrement = 30.0f;
		else
			config.ComfortModeYawIncrement = 30.0f;
	});

	menu->AddBackButtons();
	VPMENU_FinishDrawing(menu);
}

/**
* VRBoost constant value sub-menu.
***/
void D3DProxyDevice::VPMENU_VRBoostValues()
{
	SHOW_CALL("VPMENU_VRBoostValues");
	MenuBuilder *menu = VPMENU_NewFrame();
	VPMENU_StartDrawing(menu, "Settings - VRBoost");

	menu->AddAdjustment("World FOV : %g",          &VRBoostValue[24], 100.0f, 0.5f);
	menu->AddAdjustment("Player FOV : %g",         &VRBoostValue[25], 100.0f, 0.5f);
	menu->AddAdjustment("Far Plane FOV : %g",      &VRBoostValue[26], 100.0f, 0.5f);
	menu->AddAdjustment("Camera Translate X : %g", &VRBoostValue[27], 0.0f, 0.1f);
	menu->AddAdjustment("Camera Translate Y : %g", &VRBoostValue[28], 0.0f, 0.1f);
	menu->AddAdjustment("Camera Translate Z : %g", &VRBoostValue[29], 0.0f, 0.1f);
	menu->AddAdjustment("Camera Distance : %g",    &VRBoostValue[30], 0.0f, 0.1f);
	menu->AddAdjustment("Camera Zoom : %g",        &VRBoostValue[31], 0.0f, 0.1f);
	menu->AddAdjustment("Camera Horizon Adjustment : %g", &VRBoostValue[32], 0.0f, 0.1f);
	menu->AddAdjustment("Constant Value 1 : %g",   &VRBoostValue[33], 0.0f, 0.1f);
	menu->AddAdjustment("Constant Value 2 : %g",   &VRBoostValue[34], 0.0f, 0.1f);
	menu->AddAdjustment("Constant Value 2 : %g",   &VRBoostValue[35], 0.0f, 0.1f);

	menu->AddBackButtons();
	VPMENU_FinishDrawing(menu);
}

void D3DProxyDevice::VPMENU_Hotkeys()
{
	SHOW_CALL("VPMENU_Hotkeys");
	MenuBuilder *menu = VPMENU_NewFrame();
	VPMENU_StartDrawing(menu, "Settings - Hotkeys");
	
	menu->AddKeybind("Reset Orientation", &config.HotkeyResetOrientation);
	menu->AddKeybind("Show FPS Hotkey", &config.HotkeyShowFPS);
	menu->AddKeybind("Screenshot", &config.HotkeyScreenshot);
	menu->AddKeybind("Telescope Mode", &config.HotkeyTelescopeMode);
	
	menu->AddBackButtons();
	VPMENU_FinishDrawing(menu);
}

void D3DProxyDevice::VPMENU_AdjustmentHotkeys()
{
	SHOW_CALL("VPMENU_AdjustmentHotkeys");
	MenuBuilder *menu = VPMENU_NewFrame();
	VPMENU_StartDrawing(menu, "Settings - Adjustment Hotkeys");
	
	menu->AddKeybind("Swap Sides Hotkey", &config.HotkeySwapSides);
	menu->AddKeybind("Toggle Cube Renderers", &config.HotkeyToggleCubeRenders);
	menu->AddKeybind("Toggle Texture Renderers", &config.HotkeyToggleTextureRenders);
	menu->AddKeybind("Toggle When to Render Menu", &config.HotkeyWhenToRenderMenu);
	menu->AddKeybind("Toggle When to Poll Headtracking", &config.HotkeyWhenToPollHeadtracking);
	menu->AddKeybind("Initiate VRBoost Memory Scan", &config.HotkeyInitiateScan);
	menu->AddKeybind("DK2 Black Smear Correction", &config.HotkeyBlackSmear);
	menu->AddKeybind("Reset IPD Offset", &config.HotkeyResetIPDOffset);
	menu->AddKeybind("Show HMD Stats", &config.HotkeyShowHMDStats);
	menu->AddKeybind("Show Axes", &config.HotkeyShowAxes);
	menu->AddKeybind("Toggle Positional Tracking", &config.HotkeyTogglePositionalTracking);
	menu->AddKeybind("Toggle Pose Prediction", &config.HotkeyTogglePosePrediction);
	menu->AddKeybind("Toggle Chromatic Abberation Correction", &config.HotkeyToggleChromaticAbberationCorrection);
	
	menu->AddBackButtons();
	VPMENU_FinishDrawing(menu);
}


void D3DProxyDevice::VPMENU_EditKeybind(std::string description, InputBindingRef *binding)
{
	MenuBuilder *menu = VPMENU_NewFrame();
	VPMENU_StartDrawing(menu, retprintf("Bind Key - %s", description.c_str()).c_str());
	std::vector<InputBindingRef> alternatives;
	HotkeyExpressions::UnpackAlternation(*binding, &alternatives);
	
	std::string addHotkeyDescription;
	if (hotkeyCatch) {
		addHotkeyDescription = HotkeyExpressions::PackConjunction(bindingHotkeysHeld)->ToString();
	} else {
		addHotkeyDescription = "Add Hotkey";
	}
	
	menu->AddButton(addHotkeyDescription.c_str(), [=]() {
		hotkeyCatch = true;
		onBindKey = [=](InputBindingRef key) {
			std::vector<InputBindingRef> newAlternatives = alternatives;
			newAlternatives.insert(newAlternatives.begin(), key);
			*binding = HotkeyExpressions::PackAlternation(newAlternatives);
		};
	});
	
	for(size_t ii=0; ii<alternatives.size(); ii++) {
		std::string hotkeyDescription = alternatives[ii]->ToString();
		menu->AddButton(hotkeyDescription, [=]() {
			std::vector<InputBindingRef> newAlternatives = alternatives;
			newAlternatives.erase(newAlternatives.begin()+ii);
			*binding = HotkeyExpressions::PackAlternation(newAlternatives);
		});
	}
	menu->AddButton("Back", [=]() { VPMENU_Back(); });
	
	VPMENU_FinishDrawing(menu);
}


void D3DProxyDevice::VPMENU_UpdateCooldowns()
{
	SHOW_CALL("VPMENU_UpdateCooldowns");
	
	//If this is enabled, then draw an apostrophe in the top left corner of the screen at all times
	//this results in obs only picking up the left eye's texture for some reason (total hack, but some users make use of this for streaming
	//using OBS
	if (userConfig.obsStreamHack)
	{
		LPD3DXSPRITE hackSprite = NULL;
		D3DXCreateSprite(this, &hackSprite);
		if (hudFont && hackSprite)
		{
			hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);
		
			D3DXMATRIX matScale;
			D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
			hudMainMenu->SetTransform(&matScale);
			
			RECT apostropheDrawPos = { 0, 0, 50, 50 };
			hudFont->DrawText(hackSprite, "'", -1, &apostropheDrawPos, DT_LEFT, COLOR_RED);
			D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
			hackSprite->Draw(NULL, &apostropheDrawPos, NULL, &vPos, COLOR_WHITE);
			hackSprite->End();
			hackSprite->Release();
			hackSprite = NULL;
		}		
	}

	// first, calculate a time scale to adjust the menu speed for the frame speed of the game
	float timeStamp = (float)GetTickCount()/1000.0f;
	menuLastFrameLength = timeStamp-menuLastUpdateTime;
	menuLastUpdateTime = timeStamp;
	// Speed up menu - makes an incredible difference!
	float timeScale = (float)menuLastFrameLength*90;

	// menu velocity present ? in case calculate diminution of the velocity
	if (hotkeyCooldown != 0.0f)
	{
		float diminution = 0.05f * timeScale;
		if (diminution > 1.0f) diminution = 1.0f;
		hotkeyCooldown *= 1.0f-diminution;

		// set velocity to zero in case of low velocity
		if (hotkeyCooldown<0.7f && hotkeyCooldown>-0.7f)
		{
			hotkeyCooldown = 0.0f;
		}
	}

}

/**
* VP menu border velocity updated here
* Arrow up/down need to be done via call from Present().
***/
void D3DProxyDevice::VPMENU_UpdateBorder(int menuEntryCount)
{
	SHOW_CALL("VPMENU_UpdateBorder");

	// Update animation
	if(menuState.animationOffset > 0.0f) {
		menuState.animationOffset -= menuLastFrameLength * MENU_SELECT_ANIMATION_SPEED;
		if(menuState.animationOffset < 0.0f) menuState.animationOffset = 0.0f;
	}
	if(menuState.animationOffset < 0.0f) {
		menuState.animationOffset += menuLastFrameLength * MENU_SELECT_ANIMATION_SPEED;
		if(menuState.animationOffset > 0.0f) menuState.animationOffset = 0.0f;
	}
	
	// Handle up/down arrows
	float menuSelectionAxis = controls.GetAxis(InputControls::GamepadAxis::LeftStickY);
	
	if ((hotkeyMenuUp->IsPressed(controls) || (menuSelectionAxis>MENU_SELECTION_STICK_DEADZONE)) && HotkeysActive())
	{
		if(menuState.selectedIndex > 0) {
			menuState.selectedIndex--;
			menuState.animationOffset = 1.0f;
		} else {
			menuState.selectedIndex = menuEntryCount-1;
			menuState.animationOffset = 0.0f;
		}
		HotkeyCooldown(COOLDOWN_SHORT);
	}
	if ((hotkeyMenuDown->IsPressed(controls) || (menuSelectionAxis<-MENU_SELECTION_STICK_DEADZONE)) && HotkeysActive())
	{
		if(menuState.selectedIndex+1 < menuEntryCount) {
			menuState.selectedIndex++;
			menuState.animationOffset = -1.0f;
		} else {
			menuState.selectedIndex = 0;
			menuState.animationOffset = 0.0f;
		}
		HotkeyCooldown(COOLDOWN_SHORT);
	}
	
	// Update scroll position
	if(menuState.scrollOffset > menuState.selectedIndex) {
		menuState.scrollOffset = menuState.selectedIndex;
		menuState.animationOffset = 0.0f;
	}
	if(menuState.scrollOffset+MENU_SCROLL_BOTTOM < menuState.selectedIndex) {
		menuState.scrollOffset = menuState.selectedIndex - MENU_SCROLL_BOTTOM;
		menuState.animationOffset = 0.0f;
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
	config.gui3DDepthMode = (int)gui3DDepthMode;
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
	ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)config.hud3DDepthMode);

	// GUI
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
		m_fVRBoostIndicator-=menuLastFrameLength;
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

/*** MenuBuilder *************************************************************/

MenuBuilder::MenuBuilder(D3DProxyDevice *device)
{
	this->device = device;
	this->menuConstructionCurrentEntry = 0;
	RECT resetDrawPosition = { 0, 0, VPMENU_PIXEL_WIDTH, VPMENU_PIXEL_HEIGHT };
	drawPosition = resetDrawPosition;
}

void MenuBuilder::DrawItem(const char *text, D3DCOLOR color)
{
	// Only draw things within the viewport (not things scrolled off)
	if(drawPosition.top+40 >= 0
	   && drawPosition.top <= device->viewportHeight)
	{
		device->DrawTextShadowed(device->hudFont, device->hudMainMenu, text, &drawPosition, color);
	}
	drawPosition.top += MENU_ITEM_SEPARATION;
	menuConstructionCurrentEntry++;
}

void MenuBuilder::DrawItem(std::string text, D3DCOLOR color)
{
	DrawItem(text.c_str(), color);
}

void MenuBuilder::AddItem(std::string text, std::function<void()> onHover)
{
	AddItem(text, COLOR_MENU_TEXT, onHover);
}

void MenuBuilder::AddItem(std::string text, D3DCOLOR color, std::function<void()> onHover)
{
	int selection = device->VPMENU_GetCurrentSelection();
	int entryID = menuConstructionCurrentEntry;
	DrawItem(text.c_str(), color);
	if(entryID == selection)
		onHover();
}

void MenuBuilder::AddButton(std::string text, D3DCOLOR color, std::function<void()> onPick)
{
	AddItem(text, color, [=]() {
		if(device->VPMENU_Input_Selected() && device->HotkeysActive()) {
			onPick();
			device->HotkeyCooldown(COOLDOWN_SHORT);
		}
	});
}

void MenuBuilder::AddButton(std::string text, std::function<void()> onPick)
{
	AddButton(text, COLOR_MENU_TEXT, onPick);
}

void MenuBuilder::AddNavigation(std::string text, std::function<void()> menuHandler)
{
	AddItem(text, COLOR_MENU_TEXT, [=]() {
		if(device->VPMENU_Input_Selected() && device->HotkeysActive()) {
			device->VPMENU_NavigateTo(menuHandler);
		}
	});
}

/**
 * Add a config option for a key that Perception will send to the game as a
 * simulated key-press. (This is different from a Keybind, which is something
 * Perception will check on the keyboard/gamepad to see if it's pressed.)
 */
void MenuBuilder::AddGameKeypress(std::string text, byte *binding)
{
	std::string description;
	if (device->hotkeyCatch && device->VPMENU_GetCurrentSelection() == menuConstructionCurrentEntry) {
		description = "Press the desired key.";
	} else {
		description = retprintf("%s : %s",
			text.c_str(),
			Key(*binding)->ToString().c_str());
	}
	
	AddItem(description, COLOR_MENU_TEXT, [=]() {
		if (hotkeyResetToDefault->IsPressed(device->controls) && device->HotkeysActive())
		{
			*binding = 0;
			device->HotkeyCooldown(COOLDOWN_SHORT);
		}
		else if (device->VPMENU_Input_Selected() && device->HotkeysActive())
		{
			device->VPMENU_BindKey([=](InputBindingRef key) {
				SimpleKeyBinding *simpleBinding = dynamic_cast<SimpleKeyBinding*>(&*key);
				*binding = (byte)simpleBinding->GetKeyCode();
			});
			device->HotkeyCooldown(COOLDOWN_SHORT);
		}
	});
}

void MenuBuilder::AddKeybind(std::string text, InputBindingRef *binding)
{
	std::string description;
	D3DProxyDevice *device = this->device;
	description = retprintf("%s : %s", text.c_str(),
		(*binding)->ToString().c_str());
	
	AddItem(description, COLOR_MENU_TEXT, [=]() {
		if (hotkeyResetToDefault->IsPressed(device->controls) && device->HotkeysActive())
		{
			*binding = Unbound();
			device->HotkeyCooldown(COOLDOWN_SHORT);
		}
		else if (device->VPMENU_Input_Selected() && device->HotkeysActive())
		{
			device->bindingHotkeysHeld.clear();
			device->VPMENU_NavigateTo([=]() {
				device->VPMENU_EditKeybind(text, binding);
			});
			device->HotkeyCooldown(COOLDOWN_SHORT);
		}
	});
}

void MenuBuilder::AddAdjustment(const char *formatString, float *value, float defaultValue, float rate, std::function<void()> onChange)
{
	std::string label = retprintf(formatString, device->RoundVireioValue(*value));
	
	AddItem(label, [=]() {
		if (hotkeyResetToDefault->IsPressed(device->controls) && device->HotkeysActive())
		{
			*value = defaultValue;
			onChange();
			device->HotkeyCooldown(COOLDOWN_SHORT);
		}
		if (device->VPMENU_Input_IsAdjustment() && device->HotkeysActive())
		{
			*value += rate * device->VPMENU_Input_GetAdjustment();
			onChange();
			device->HotkeyCooldown(COOLDOWN_SHORT);
		}
	});
}

void MenuBuilder::AddEnumPicker(const char *formatString, int *currentValue, int maxValue, std::function<std::string(int)> getDescription, std::function<void(int)> onChange, std::function<void()> onActivate)
{
	std::string description = retprintf(formatString, getDescription(*currentValue).c_str());
	
	int indicatorTop = (int)(GetDrawPositionTop() * device->fScaleY);
	AddItem(description, [=]()
	{
		if (device->VPMENU_Input_Left() && device->HotkeysActive())
		{
			if (*currentValue > 0) {
				(*currentValue)--;
				onChange(*currentValue);
			}
			device->HotkeyCooldown(COOLDOWN_SHORT);
		}
		if (device->VPMENU_Input_Right() && device->HotkeysActive())
		{
			if (*currentValue+1 < maxValue) {
				(*currentValue)++;
				onChange(*currentValue);
			}
			device->HotkeyCooldown(COOLDOWN_SHORT);
		}
		if (device->VPMENU_Input_Selected() && device->HotkeysActive())
		{
			onActivate();
			device->HotkeyCooldown(COOLDOWN_SHORT);
		}
	});
	
	D3DRECT rect;
	rect.x1 = (int)(device->viewportWidth*0.59f);
	rect.x2 = (int)(device->viewportWidth*0.63f);
	rect.y1 = indicatorTop;
	rect.y2 = (int)(indicatorTop+device->viewportHeight*0.027f);
	device->DrawSelection(vireio::RenderPosition::Left, rect, COLOR_QUICK_SETTING, *currentValue, maxValue);
	device->DrawSelection(vireio::RenderPosition::Right, rect, COLOR_QUICK_SETTING, *currentValue, maxValue);
}

void MenuBuilder::AddBackButtons()
{
	AddButton("Back to Main Menu", [=]() {
		device->VPMENU_Back();
	});
	AddButton("Back to Game", [=]() {
		device->VPMENU_Close();
	});
}

void MenuBuilder::OnClose(std::function<void()> onClose)
{
	device->menuState.onClose = onClose;
}

void MenuBuilder::ResetDrawPosition()
{
	drawPosition.left = 800;
	drawPosition.top = 350 - device->menuState.scrollOffset*MENU_ITEM_SEPARATION;
}

int MenuBuilder::GetDrawPositionTop()
{
	return drawPosition.top;
}

int MenuBuilder::GetEntryCount()
{
	return menuConstructionCurrentEntry;
}

/*** MenuState ***************************************************************/

MenuState::MenuState(D3DProxyDevice *device)
{
	this->device = device;
	this->onClose = [](){};
	Reset();
}

void MenuState::Reset()
{
	selectedIndex = 0;
	scrollOffset = 0;
	animationOffset = 0.0f;
	borderTopHeight = 0.0f;
}
