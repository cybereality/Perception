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

#ifdef x64
#define PR_SIZET "I64"
#else
#define PR_SIZET ""
#endif


using namespace VRBoost;
using namespace vireio;
using namespace HotkeyExpressions;

InputBindingRef LShift = Key(VK_LSHIFT);
InputBindingRef LCtrl = Key(VK_LCONTROL);
InputBindingRef LAlt = Key(VK_MENU);

InputBindingRef RCtrl = Key(VK_RCONTROL);

InputBindingRef hotkeyCrouch = Button(0xc) || Key(VK_RSHIFT);
InputBindingRef hotkeySkipProne = Button(0xd) || Key(VK_ESCAPE);

InputBindingRef hotkeyNextScanCandidate = Key(VK_NUMPAD6) || (LCtrl+Key(VK_OEM_PERIOD));
InputBindingRef hotkeyPrevScanCandidate = Key(VK_NUMPAD4) || (LCtrl+Key(VK_OEM_COMMA));
InputBindingRef hotkeyCancelScan = Key(VK_NUMPAD8) || (LCtrl + Key(VK_OEM_1));

InputBindingRef hotkeyDoubleClickVPMenu = Button(4);
InputBindingRef hotkeyOpenVPMenu = (LCtrl+Key('Q')) || (LShift+Key(VK_MULTIPLY));

InputBindingRef hotkeyWheelYOffset = LCtrl+Key(VK_TAB);
InputBindingRef hotkeyWheelIPDOffset = LCtrl+LShift;
InputBindingRef hotkeyWheelWorldScale = LCtrl+LAlt;
InputBindingRef hotkeyWheelZBuffer = LCtrl+Key(0x5A);
InputBindingRef hotkeyWheelPFOV = LCtrl+Key(VK_SPACE);
InputBindingRef hotkeyWheelZoomScale = LCtrl;

/**
* Keyboard input handling
***/
void D3DProxyDevice::HandleControls()
{
	SHOW_CALL("HandleControls");
	
	controls.UpdateInputs();

	if(config.hudSwitchHotkey->IsPressed(controls))
	{
		HUD_3D_Depth_Modes newMode=(HUD_3D_Depth_Modes)(hud3DDepthMode+1);
		if (newMode>=HUD_3D_Depth_Modes::HUD_ENUM_RANGE)
			newMode=HUD_3D_Depth_Modes::HUD_DEFAULT;
		oldHudMode = hud3DDepthMode;
		ChangeHUD3DDepthMode(newMode);
	}
	
	if(config.guiSwitchHotkey->IsPressed(controls))
	{
		GUI_3D_Depth_Modes newMode=(GUI_3D_Depth_Modes)(gui3DDepthMode+1);
		if (newMode>=GUI_3D_Depth_Modes::GUI_ENUM_RANGE)
			newMode=GUI_3D_Depth_Modes::GUI_DEFAULT;
		oldGuiMode = gui3DDepthMode;
		ChangeGUI3DDepthMode(newMode);
	}
	
	// loop through hotkeys
	for (int i=0; i<4; i++)
	{
		if (config.hudHotkeys[i]->IsPressed(controls) && HotkeysActive())
		{
			if (hud3DDepthMode==(HUD_3D_Depth_Modes)i)
			{
				if (RCtrl->IsHeld(controls))
				{
					oldHudMode = hud3DDepthMode;
					ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)i);
				}
				else
				{
					ChangeHUD3DDepthMode(oldHudMode);
				}
			}
			else
			{
				oldHudMode = hud3DDepthMode;
				ChangeHUD3DDepthMode((HUD_3D_Depth_Modes)i);
			}
		}
		if (config.guiHotkeys[i]->IsPressed(controls) && HotkeysActive())
		{
			if (gui3DDepthMode==(GUI_3D_Depth_Modes)i)
			{
				if (RCtrl->IsHeld(controls))
				{
					oldGuiMode = gui3DDepthMode;
					ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)i);
				}
				else
				{
					ChangeGUI3DDepthMode(oldGuiMode);
				}
			}
			else
			{
				oldGuiMode = gui3DDepthMode;
				ChangeGUI3DDepthMode((GUI_3D_Depth_Modes)i);
			}
		}
	}

	// test VRBoost reset hotkey
	if (config.VRBoostResetHotkey->IsPressed(controls) && HotkeysActive())
	{
		if (hmVRboost!=NULL)
		{
			m_pVRboost_ReleaseAllMemoryRules();
			m_bVRBoostToggle = !m_bVRBoostToggle;
			if (tracker->getStatus() > MTS_OK)
				tracker->resetOrientationAndPosition();

			// set the indicator to be drawn
			m_fVRBoostIndicator = 1.0f;

		}
	}

	//If we are in comfort mode and user has pushed left or right, then change yaw
	if (VRBoostStatus.VRBoost_Active &&
		HotkeysActive())
	{
		if (config.ComfortModeLeftKey->IsPressed(controls))
		{
			m_comfortModeYaw +=config.ComfortModeYawIncrement;
			if (m_comfortModeYaw == 180.0f)
				m_comfortModeYaw = -180.0f;
		}

		if (config.ComfortModeRightKey->IsPressed(controls))
		{
			m_comfortModeYaw -= config.ComfortModeYawIncrement;
			if (m_comfortModeYaw == -180.0f)
				m_comfortModeYaw = 180.0f;
		}
	}

	//Double clicking the Right-Ctrl will disable or re-enable all Vireio hot-keys
	static DWORD rctrlStartClick = 0;
	if ((RCtrl->IsHeld(controls) || rctrlStartClick != 0) && HotkeysActive())
	{
		if (RCtrl->IsHeld(controls) && rctrlStartClick == 0)
		{
			rctrlStartClick = 1;
		}
		else if (!RCtrl->IsHeld(controls) && rctrlStartClick == 1)
		{
			rctrlStartClick = GetTickCount();
		}
		else if (RCtrl->IsHeld(controls) && rctrlStartClick > 1)
		{
			//If we clicked a second time within 500 ms
			if ((GetTickCount() - rctrlStartClick) <= 500)
			{
				if (!m_disableAllHotkeys)
				{
					m_disableAllHotkeys = true;
					ShowAdjusterToast("VIREIO HOT-KEYS: DISABLED", 2000);
				}
				else
				{
					m_disableAllHotkeys = false;
					ShowAdjusterToast("VIREIO HOT-KEYS: ENABLED", 2000);
				}
			}

			rctrlStartClick = 0;
		}
		else if (rctrlStartClick > 1 && 
			(GetTickCount() - rctrlStartClick) > 500)
		{
			//Reset, user clearly not double clicking
			rctrlStartClick = 0;
		}
	}

	//Disconnected Screen View Mode
	if (config.EdgePeekHotkey->IsPressed(controls) && HotkeysActive())
	{
		static bool bSurpressPositionaltracking = true;
		static bool bForceMouseEmulation = false;
		if (m_bfloatingScreen)
		{
			m_bfloatingScreen = false;
			m_bSurpressHeadtracking = false;
			tracker->setMouseEmulation(bForceMouseEmulation);
			bSurpressPositionaltracking = m_bSurpressPositionaltracking;
			m_bSurpressPositionaltracking = false;
			//TODO Change this back to initial
			this->stereoView->HeadYOffset = 0;
			this->stereoView->HeadZOffset = FLT_MAX;
			this->stereoView->XOffset = 0;
			this->stereoView->PostReset();	
		}
		else
		{
			//Suspend in-game movement whilst showing disconnected screen view
			m_bfloatingScreen = true;
			m_bSurpressHeadtracking = true;
			bForceMouseEmulation = tracker->setMouseEmulation(false);
			m_bSurpressPositionaltracking = bSurpressPositionaltracking;
			if (tracker->getStatus() >= MTS_OK)
			{
				m_fFloatingScreenPitch = tracker->primaryPitch;
				m_fFloatingScreenYaw = tracker->primaryYaw;
				m_fFloatingScreenZ = tracker->z;
			}
		}

		ShowAdjusterToast(retprintf("Disconnected Screen %s",
			m_bfloatingScreen ? "Enabled" : "Disabled"), 700);
	}

	float screenFloatMultiplierY = 0.75;
	float screenFloatMultiplierX = 0.5;
	float screenFloatMultiplierZ = 1.5;
	if(m_bfloatingScreen)
	{
		if (tracker->getStatus() >= MTS_OK)
		{
			this->stereoView->HeadYOffset = (m_fFloatingScreenPitch - tracker->primaryPitch) * screenFloatMultiplierY + (0.5f * tracker->y);
			this->stereoView->XOffset = (m_fFloatingScreenYaw - tracker->primaryYaw) * screenFloatMultiplierX + (0.5f * tracker->x);
			this->stereoView->HeadZOffset = (m_fFloatingScreenZ - tracker->z) * screenFloatMultiplierZ;
			this->stereoView->PostReset();
		}
	}
	else
	{
		if (this->stereoView->m_screenViewGlideFactor < 1.0f)
		{
			float drift = (sinf(1 + (-cosf((1.0f - this->stereoView->m_screenViewGlideFactor) * 3.142f) / 2)) - 0.5f) * 2.0f;
			this->stereoView->HeadYOffset = ((m_fFloatingScreenPitch - tracker->primaryPitch) * screenFloatMultiplierY) 
				* drift;
			this->stereoView->XOffset = ((m_fFloatingScreenYaw - tracker->primaryYaw) * screenFloatMultiplierX) 
				* drift;

			this->stereoView->PostReset();
		}
	}

	//Anything in the following block will be unavailable whilst disable hot-keys is active
	if (!m_disableAllHotkeys)
	{
		//Rset HMD Orientation+Position LSHIFT+R, or L+R Shoulder buttons on Xbox 360 controller
		if (config.HotkeyResetOrientation->IsPressed(controls) && HotkeysActive())
		{
			if (calibrate_tracker)
			{
				calibrate_tracker = false;
				//Replace popup
				DismissPopup(VPT_CALIBRATE_TRACKER);
				ShowPopup(VPT_NOTIFICATION, VPS_INFO, 3000,
					"\n\nHMD Orientation and Position Calibrated\n"
					"Please repeat if required...");
			}
			else
			{
				ShowPopup(VPT_NOTIFICATION, VPS_TOAST, 1200, "HMD Orientation and Position Reset");
			}
			tracker->resetOrientationAndPosition();
		}

		//Duck and cover - trigger crouch and prone keys if Y position of HMD moves appropriately
		if (m_DuckAndCover.dfcStatus > DAC_INACTIVE &&
			m_DuckAndCover.dfcStatus < DAC_DISABLED && tracker &&
			tracker->getStatus() >= MTS_OK)
		{
			if (hotkeyCrouch->IsPressed(controls) && HotkeysActive())
			{
				if (m_DuckAndCover.dfcStatus == DAC_CAL_STANDING)
				{
					//Reset positional ready for the next stage
					tracker->resetPosition();
					m_DuckAndCover.dfcStatus = DAC_CAL_CROUCHING;
				}
				else if (m_DuckAndCover.dfcStatus == DAC_CAL_CROUCHING)
				{
					m_DuckAndCover.yPos_Crouch = tracker->y;
					//Slightly randomly decided on this
					m_DuckAndCover.yPos_Jump = fabs(tracker->y) / 3.0f;
					m_DuckAndCover.dfcStatus = DAC_CAL_PRONE;
				}
				else if (m_DuckAndCover.dfcStatus == DAC_CAL_PRONE)
				{
					m_DuckAndCover.proneEnabled = true;
					m_DuckAndCover.yPos_Prone = tracker->y - m_DuckAndCover.yPos_Crouch;
					m_DuckAndCover.dfcStatus = DAC_CAL_COMPLETE;
				}
				else if (m_DuckAndCover.dfcStatus == DAC_CAL_COMPLETE)
				{
					//Ready to go..
					m_DuckAndCover.dfcStatus = DAC_STANDING;
					tracker->resetPosition();
					DismissPopup(VPT_NOTIFICATION);
				}
			}
			//B button only skips the prone position
			else if (hotkeySkipProne->IsPressed(controls) && HotkeysActive())
			{
				if (m_DuckAndCover.dfcStatus == DAC_CAL_PRONE)
				{
					m_DuckAndCover.proneEnabled = false;
					m_DuckAndCover.dfcStatus = DAC_CAL_COMPLETE;
				}
			}
		}


		// Show active VRBoost axes and their addresses (SHIFT+V)
		if (config.HotkeyShowAxes->IsPressed(controls) && HotkeysActive())
		{
			if (hmVRboost!=NULL)
			{
				if (VRBoostStatus.VRBoost_Active)
				{
					ActiveAxisInfo axes[30];
					memset(axes, 0xFF, sizeof(ActiveAxisInfo) * 30);
					UINT count = m_pVRboost_GetActiveRuleAxes((ActiveAxisInfo**)&axes);
					std::string popupMessage = retprintf("VRBoost Axis Addresses: %i\n", count);

					UINT i = 0;
					while (i < count)
					{
						if (axes[i].Axis == MAXDWORD || i == 6)
							break;

						std::string axisName = VRboostAxisString(axes[i].Axis);
						popupMessage += retprintf(
							"      %s:      0x%"PR_SIZET"x\n",
							axisName.c_str(), axes[i].Address);

						i++;
					}
					
					ShowPopup(VPT_NOTIFICATION, VPS_INFO, 10000, popupMessage);
				}
			}
		}

		// switch to 2d Depth Mode (Shift + O / Numpad 9)
		if (config.HotkeySwitch2DDepthMode->IsPressed(controls) && HotkeysActive())
		{
			m_3DReconstructionMode++;
			if (m_3DReconstructionMode == 4)
				m_3DReconstructionMode = Reconstruction_Type::GEOMETRY;
			stereoView->m_3DReconstructionMode = m_3DReconstructionMode;
			if(m_3DReconstructionMode == Reconstruction_Type::GEOMETRY)
				ShowAdjusterToast(retprintf("Depth Perception Mode %s","Geometry"), 1000);
			else if(m_3DReconstructionMode == Reconstruction_Type::ZBUFFER)
				ShowAdjusterToast(retprintf("Depth Perception Mode %s","Z Buffer"), 1000);
			else
				ShowAdjusterToast(retprintf("Depth Perception Mode %s","Mono"), 1000);
		}

		// Swap Sides on Depth mode (Alt + O)
		if (config.HotkeySwapSides->IsPressed(controls) && HotkeysActive())
		{
			stereoView->m_bLeftSideActive = !stereoView->m_bLeftSideActive;
			ShowAdjusterToast("Depth Perception Side Switched", 1000);			
		}

		// cycle Render States
		if ((config.HotkeyNextRenderState->IsPressed(controls) || config.HotkeyPrevRenderState->IsPressed(controls))&& HotkeysActive())
		{
			std::string _str = "";
			if(config.HotkeyPrevRenderState->IsPressed(controls))
			{
				_str = stereoView->CycleRenderState(false);
			}
			else if(config.HotkeyNextRenderState->IsPressed(controls))
			{
				_str = stereoView->CycleRenderState(true);
			}
			ShowPopup(VPT_ADJUSTER, VPS_TOAST, 1000,
				retprintf("New Render State :: %s", _str.c_str()));
		}

		// Toggle Through Cube Renders -> ALt + 1
		if (config.HotkeyToggleCubeRenders->IsPressed(controls) && HotkeysActive())
		{
			const char *cubeDuplicationDescription = "?";
			if(m_pGameHandler->intDuplicateCubeTexture < 3)
			{
				m_pGameHandler->intDuplicateCubeTexture++;
				if(m_pGameHandler->intDuplicateCubeTexture == 1)
					cubeDuplicationDescription = "Always False";
				else if(m_pGameHandler->intDuplicateCubeTexture == 2)
					cubeDuplicationDescription = "Always True";
				else if(m_pGameHandler->intDuplicateCubeTexture == 3)
					cubeDuplicationDescription = "IS_RENDER_TARGET(Usage)";
			}
			else
			{
				m_pGameHandler->intDuplicateCubeTexture = 0;
				cubeDuplicationDescription = "Default (Game Type)";
			}
		
			ShowPopup(VPT_ADJUSTER, VPS_TOAST, 1000,
				retprintf("Cube Duplication :: %s", cubeDuplicationDescription));
		}
		// Toggle Through Texture Renders -> ALt + 2
		if (config.HotkeyToggleTextureRenders->IsPressed(controls) && HotkeysActive())
		{
			const char *textureDuplicationDescription = "?";
			if(m_pGameHandler->intDuplicateTexture < 4)
			{
				m_pGameHandler->intDuplicateTexture++;
				if(m_pGameHandler->intDuplicateTexture == 1)
					textureDuplicationDescription = "Always False";
				else if(m_pGameHandler->intDuplicateTexture == 2)
					textureDuplicationDescription = "Always True";
				else if(m_pGameHandler->intDuplicateTexture == 3)
					textureDuplicationDescription = "IS_RENDER_TARGET";
				else if(m_pGameHandler->intDuplicateTexture == 4)
					textureDuplicationDescription = "IS_RENDER_TARGET + Width and Height)";
			}
			else
			{
				m_pGameHandler->intDuplicateTexture = 0;
				textureDuplicationDescription = "Default (Game Type)";
			}
		
			ShowPopup(VPT_ADJUSTER, VPS_TOAST, 1000,
				retprintf("Texture Duplication :: %s", textureDuplicationDescription));
		}

		//When to render vpmenu (Alt + Up)
		if (config.HotkeyWhenToRenderMenu->IsPressed(controls) && HotkeysActive())
		{
			const char *vpmenuRenderDesc = "?";
			if(m_deviceBehavior.whenToRenderVPMENU == DeviceBehavior::BEGIN_SCENE)
			{
				m_deviceBehavior.whenToRenderVPMENU = DeviceBehavior::END_SCENE;
				vpmenuRenderDesc = "END_SCENE";
			}
			else if(m_deviceBehavior.whenToRenderVPMENU == DeviceBehavior::END_SCENE)
			{
				m_deviceBehavior.whenToRenderVPMENU = DeviceBehavior::BEFORE_COMPOSITING;
				vpmenuRenderDesc = "BEFORE_COMPOSITING";
			}
			else
			{
				m_deviceBehavior.whenToRenderVPMENU = DeviceBehavior::BEGIN_SCENE;
				vpmenuRenderDesc = "BEGIN_SCENE";
			}
			
			ShowPopup(VPT_ADJUSTER, VPS_TOAST, 1000,
				retprintf("VPMENU RENDER = %s", vpmenuRenderDesc));
		}

		//When to poll headtracking (Alt + Down)
		if (config.HotkeyWhenToPollHeadtracking->IsPressed(controls) && HotkeysActive())
		{
			std::string popupMessage = "";
			
			if(m_deviceBehavior.whenToHandleHeadTracking == DeviceBehavior::BEGIN_SCENE)
			{
				m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::END_SCENE;
				popupMessage = "\n\nHEADTRACKING = END_SCENE";
			}
			else if(m_deviceBehavior.whenToHandleHeadTracking == DeviceBehavior::END_SCENE)
			{
				m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::BEGIN_SCENE;
				popupMessage = "\n\nHEADTRACKING = BEGIN_SCENE";
			}
			/*else if(m_deviceBehavior.whenToHandleHeadTracking == DeviceBehavior::BEFORE_COMPOSITING)
			{
				m_deviceBehavior.whenToHandleHeadTracking = DeviceBehavior::BEGIN_SCENE;
				popupMessage = "\n\nHEADTRACKING = BEGIN_SCENE";
			}//TODO This Crashes for some reason - problem for another day*/
		
			ShowPopup(VPT_ADJUSTER, VPS_TOAST, 1000, popupMessage);
		}

		// Initiate VRBoost Memory Scan (NUMPAD5 or <LCTRL> + </> )
		if (config.HotkeyInitiateScan->IsPressed(controls) && HotkeysActive())
		{
			if (hmVRboost!=NULL)
			{
				//Use local static, as it is a simple flag
				static bool showRescanWarning = false;
				static bool shownRescanWarning = false;
				if (showRescanWarning && !VRBoostStatus.VRBoost_Scanning)
				{
					//If roll isn't enabled then rolling is done through the game engine using VRBoost
					//In this case, if the user has already run a successful scan then running again would likely
					//fail as the roll address will almost definitely not be 0, which is what the scanner would be looking for
					//so before starting the scan, confirm with the user this is what they actually wish to do
					//This will also prevent an accidental re-run
					//Games that use matrix roll can usually be re-run without issue
					ShowPopup(VPT_NOTIFICATION, VPS_INFO, 5000,
						"   *WARNING*: re-running a scan once stable\n"
						"   addresses have been found could fail\n"
						"   IF NO SCAN HAS YET SUCCEEDED; IGNORE THIS WARNING\n"
						"   Press scan trigger again to initiate scan\n"
						"   or wait for this message to disappear (No Scan)");
					showRescanWarning = false;
					shownRescanWarning = true;
				}
				else
				{
					//Ensure the previous notification is dismissed
					if (shownRescanWarning)
					{
						DismissPopup(VPT_NOTIFICATION);
						shownRescanWarning = false;
					}

					ReturnValue vr = m_pVRboost_StartMemoryScan();
					if (vr == VRBOOST_ERROR)
					{
						ShowPopup(VPT_VRBOOST_FAILURE, VPS_TOAST, 5000,
							"VRBoost: StartMemoryScan - Failed");
					}
					//If initialising then we have successfully started a new scan
					else if (vr = VRBOOST_SCAN_INITIALISING)
					{
						VRBoostStatus.VRBoost_Scanning = true;
						//Definitely have no candidates at this point
						VRBoostStatus.VRBoost_Candidates = false;
						showRescanWarning = true;
					}
				}
			}
		}

		// Select next scan candidate if there is one
		//  Increase = NUMPAD6 or <LCTRL> + <.> 
		//  Decrease = NUMPAD4 or <LCTRL> + <,> 
		if (VRBoostStatus.VRBoost_Candidates && 
			(hotkeyNextScanCandidate->IsPressed(controls) || hotkeyPrevScanCandidate->IsPressed(controls)) && 
			HotkeysActive())
		{
			if (hmVRboost!=NULL)
			{
				static int c = 0;
				UINT candidates = m_pVRboost_GetScanCandidates();
				bool increase = hotkeyNextScanCandidate->IsPressed(controls);
				if (increase)
					c = (c + 1) % candidates;
				else
				{
					if (--c < 0) c = candidates - 1;
				}

				m_pVRboost_SetNextScanCandidate(increase);
				DismissPopup(VPT_NOTIFICATION);
				ShowPopup(VPT_NOTIFICATION, VPS_TOAST, 1000,
					retprintf("VRBoost: Select Next Scan Candidate: %i / %i",
						c+1, candidates));
			}
		}
	
		// Cancel VRBoost Memory Scan Mode (NUMPAD8 or <LCTRL> + <;> )
		if (hotkeyCancelScan->IsPressed(controls) && HotkeysActive())
		{
			DismissPopup(VPT_VRBOOST_SCANNING);

			VRBoostStatus.VRBoost_Scanning = false;
			VRBoostStatus.VRBoost_Candidates = false;

			m_bForceMouseEmulation = true;
			tracker->setMouseEmulation(true);
		}

		//Enabled/Disable Free Pitch (default is disabled), LSHIFT + X
		if (VRBoostStatus.VRBoost_Active && 
			config.HotkeyToggleFreePitch->IsPressed(controls) &&
			HotkeysActive())
		{
			if (VRBoostValue[VRboostAxis::FreePitch] != 0.0f)
			{
				//Disable Free Pitch
				VRBoostValue[VRboostAxis::FreePitch] = 0.0f;
			}
			else
			{
				//Enable Free Pitch
				VRBoostValue[VRboostAxis::FreePitch] = 1.0f;
			}
			
			ShowAdjusterToast(retprintf("Pitch Free-look Enabled",
				(VRBoostValue[VRboostAxis::FreePitch] != 0.0f) ? "Enabled" : "Disabled"), 1000);
		}

		//Enabled/Disable Black Smear Correction for DK2 (default is disabled), LSHIFT + B
		if ((tracker && tracker->SupportsPositionTracking()) &&
			config.HotkeyBlackSmear->IsPressed(controls) && 
			HotkeysActive())
		{
			if (stereoView->m_blackSmearCorrection != 0.0f)
				stereoView->m_blackSmearCorrection = 0.0f;
			else
				stereoView->m_blackSmearCorrection = 0.02f;
			
			stereoView->PostReset();
			ShowAdjusterToast(retprintf("DK2 Black Smear Correction %s",
				stereoView->m_blackSmearCorrection!=0.0f ? "Enabled" : "Disabled"), 1000);
		}


		//Reset IPD Offset to 0  -  F8  or  LSHIFT+I
		if (config.HotkeyResetIPDOffset->IsPressed(controls) && HotkeysActive() && config.IPDOffset != 0.0)
		{
			config.IPDOffset = 0.0;
			this->stereoView->PostReset();		

			ShowAdjusterToast(retprintf("IPD-Offset: %1.3f", config.IPDOffset), 500);
			DeferedSaveConfig();
		}

		//Show FPS Counter / Frame Time counter LSHIFT+F
		if (config.HotkeyShowFPS->IsPressed(controls) && HotkeysActive())
		{
			show_fps = (FPS_TYPE)((show_fps+1) % 3);
		}

		//Show HMD Stats Counter LSHIFT+H 
		if (config.HotkeyShowHMDStats->IsPressed(controls) && HotkeysActive())
		{
			if (activePopup.popupType == VPT_STATS)
			{
				DismissPopup(VPT_STATS);
			}
			else
			{
				VireioPopup popup(VPT_STATS);
				ShowPopup(popup);
			}
		}

		//Toggle positional tracking
		if (config.HotkeyTogglePositionalTracking->IsPressed(controls) && HotkeysActive())
		{
			m_bPosTrackingToggle = !m_bPosTrackingToggle;

			ShowPopup(VPT_NOTIFICATION, VPS_TOAST, 1200,
				retprintf("HMD Positional Tracking %s",
					m_bPosTrackingToggle?"Enabled":"Disabled"));

			if (!m_bPosTrackingToggle)
				m_spShaderViewAdjustment->UpdatePosition(0.0f, 0.0f, 0.0f);
		}

		//Toggle SDK Pose Prediction- LSHIFT + DELETE
		if (hmdInfo->GetHMDManufacturer() == HMD_OCULUS	&&
			config.HotkeyTogglePosePrediction->IsPressed(controls) && HotkeysActive() && tracker)
		{
			tracker->useSDKPosePrediction = !tracker->useSDKPosePrediction;

			ShowPopup(VPT_NOTIFICATION, VPS_TOAST, 1200,
				retprintf("SDK Pose Prediction %s",
					tracker->useSDKPosePrediction?"Enabled":"Disabled"));
		}

		//Toggle chromatic abberation correction - SHIFT+J
		if (config.HotkeyToggleChromaticAbberationCorrection->IsPressed(controls) && HotkeysActive())
		{
			stereoView->chromaticAberrationCorrection = !stereoView->chromaticAberrationCorrection;

			ShowPopup(VPT_NOTIFICATION, VPS_TOAST, 1200,
				retprintf("Chromatic Aberration Correction %s",
					stereoView->chromaticAberrationCorrection?"Enabled":"Disabled"));
		}

		//Double clicking the NUMPAD0 key will invoke the VR mouse
		//Double clicking when VR mouse is enabled will either:
		//   - Toggle between GUI and HUD scaling if double click occurs within 2 seconds
		//   - Disable VR Mouse if double click occurs after 2 seconds
		static DWORD numPad0Click = 0;
		if ((config.HotkeyVRMouse->IsPressed(controls) || numPad0Click != 0) && HotkeysActive())
		{
			if (config.HotkeyVRMouse->IsPressed(controls) && numPad0Click == 0)
			{
				numPad0Click = 1;
			}
			else if (!config.HotkeyVRMouse->IsPressed(controls) && numPad0Click == 1)
			{
				numPad0Click = GetTickCount();
			}
			else if (config.HotkeyVRMouse->IsPressed(controls) && numPad0Click > 1)
			{
				//If we clicked a second time within 500 ms, then trigger VR Mouse
				if ((GetTickCount() - numPad0Click) <= 500)
				{
					static DWORD tc = 0;
					if (tc != 0 && (GetTickCount() - tc > 2000))
					{
						tc = 0;
						m_showVRMouse = 0;
						stereoView->m_mousePos.x = 0;
						stereoView->m_mousePos.y = 0;
					}
					else
					{
						tc = GetTickCount();
						if (m_showVRMouse == 2)
							m_showVRMouse = 1;
						else
							m_showVRMouse++;
					}

					stereoView->PostReset();

					DismissPopup(VPT_NOTIFICATION);
					
					const char *vrMouseDescription;
					if (m_showVRMouse == 1)
						vrMouseDescription = "GUI Scaling";
					else if (m_showVRMouse == 2)
						vrMouseDescription = "HUD Scaling";
					else
						vrMouseDescription = "Disabled";
					ShowPopup(VPT_NOTIFICATION, VPS_TOAST, 1200,
						retprintf("VR Mouse - %s", vrMouseDescription));
				}

				numPad0Click = 0;
			}
			else if (numPad0Click > 1 &&
				(GetTickCount() - numPad0Click) > 500)
			{
				//Reset, user clearly not double clicking
				numPad0Click = 0;
			}
		}
		
		// floaty menus
		if (config.HotkeyFloatyMenus->IsPressed(controls) && HotkeysActive())
		{
			if (m_bfloatingMenu)
				m_bfloatingMenu = false;
			else
			{
				m_bfloatingMenu = true;
				if (tracker->getStatus() >= MTS_OK)
				{
					m_fFloatingPitch = tracker->primaryPitch;
					m_fFloatingYaw = tracker->primaryYaw;			
				}
			}

			ShowPopup(VPT_NOTIFICATION, VPS_TOAST, 1200,
				retprintf("Floating Menus %s",
					m_bfloatingMenu?"Enabled":"Disabled"));
		}

		//Double clicking the start button will invoke the VP menu
		static DWORD startClick = 0;
		if ((hotkeyDoubleClickVPMenu->IsPressed(controls) || startClick != 0) && HotkeysActive())
		{
			if (hotkeyDoubleClickVPMenu->IsPressed(controls) && startClick == 0)
			{
				startClick = 1;
			}
			else if (!hotkeyDoubleClickVPMenu->IsPressed(controls) && startClick == 1)
			{
				startClick = GetTickCount();
			}
			else if (hotkeyDoubleClickVPMenu->IsPressed(controls) && startClick > 1)
			{
				//If we clicked a second time within 500 ms, then open vp menu
				if ((GetTickCount() - startClick) <= 500)
				{
					if (!VPMENU_IsOpen())
						VPMENU_OpenMainMenu();
					else
						VPMENU_Close();
				}

				startClick = 0;
			}
			else if (startClick > 1 &&
				(GetTickCount() - startClick) > 500)
			{
				//Reset, user clearly not double clicking
				startClick = 0;
			}
		}

		// open VP Menu - <CTRL>+<Q>
		if(hotkeyOpenVPMenu->IsPressed(controls) && HotkeysActive())
		{
			if (!VPMENU_IsOpen())
				VPMENU_OpenMainMenu();
			else
				VPMENU_Close();
		}

		//Mouse Wheel Scroll
		int _wheel = dinput.GetWheel();
		if(_wheel != 0)
		{
			int wheelSign = (_wheel>0) ? 1 : -1;
			if(hotkeyWheelYOffset->IsHeld(controls))
			{
				config.YOffset += 0.005f * wheelSign;
				clamp(&config.YOffset, -0.1f, 0.1f);
				
				this->stereoView->PostReset();
				DeferedSaveConfig();
				ShowAdjusterToast(retprintf("Y-Offset: %1.3f", config.YOffset), 500);
			}
			else if(hotkeyWheelZBuffer->IsHeld(controls))
			{
				if(stereoView->m_bZBufferFilterMode)
				{
					this->stereoView->m_fZBufferFilter += 0.0005f * wheelSign;
					clamp(&this->stereoView->m_fZBufferFilter, 0.01f, 1.0f);
					this->stereoView->PostReset();
					ShowAdjusterToast(retprintf("ZBuffer Depth Filter: %1.6f", this->stereoView->m_fZBufferFilter), 500);
				}
				else if(stereoView->m_bZBufferVisualisationMode)
				{
					config.zbufferDepthLow += 0.0005f * wheelSign;
					clamp(&config.zbufferDepthLow, 0.01f, 1.0f);					
					DeferedSaveConfig();
					ShowAdjusterToast(retprintf("ZBuffer Depth Bound Low: %1.6f", config.zbufferDepthLow), 500);
				}
				else
				{
					config.zbufferStrength += 1.0000f * wheelSign;
					DeferedSaveConfig();
					ShowAdjusterToast(retprintf("ZBuffer Seperation Strength: %1.6f", config.zbufferStrength), 500);
				}												
			}
			else if(hotkeyWheelIPDOffset->IsHeld(controls))
			{
				config.IPDOffset += 0.001f * wheelSign;
				clamp(&config.IPDOffset, -0.1f, 0.1f);

				this->stereoView->PostReset();
				ShowAdjusterToast(retprintf("IPD-Offset: %1.3f", config.IPDOffset), 500);
				DeferedSaveConfig();
			}
			//CTRL + ALT + Mouse Wheel - adjust World Scale dynamically
			else if (hotkeyWheelWorldScale->IsHeld(controls))
			{
				float separationChange = 0.05f * wheelSign;
				m_spShaderViewAdjustment->ChangeWorldScale(separationChange);

				m_spShaderViewAdjustment->UpdateProjectionMatrices((float)stereoView->viewport.Width/(float)stereoView->viewport.Height, config.PFOV);
				ShowAdjusterToast(retprintf("Stereo Separation (World Scale): %1.3f", config.worldScaleFactor), 500);
				DeferedSaveConfig();
			}
			//CTRL + SPACE + Mouse Wheel - adjust projection fov dynamically
			else if(hotkeyWheelPFOV->IsHeld(controls))
			{
				config.PFOV += 0.5f*wheelSign;

				m_spShaderViewAdjustment->UpdateProjectionMatrices((float)stereoView->viewport.Width/(float)stereoView->viewport.Height, config.PFOV);
				
				ShowAdjusterToast(retprintf("Projection FOV: %1.3f", config.PFOV), 500);
				DeferedSaveConfig();
			}
			else if(hotkeyWheelZoomScale->IsHeld(controls))
			{
				this->stereoView->ZoomOutScale += 0.05f * wheelSign;
				clamp(&this->stereoView->ZoomOutScale, 0.05f, 2.00f);
				
				this->stereoView->PostReset();
				DeferedSaveConfig();
				ShowAdjusterToast(retprintf("Zoom Scale: %1.3f", this->stereoView->ZoomOutScale), 500);
			}
		}
	
		//Change Distortion Scale CTRL + + / -
		if(config.HotkeyDistortionScalePlus->IsPressed(controls) && HotkeysActive())
		{
			this->stereoView->ZoomOutScale = 1.00f;
			this->stereoView->PostReset();	

			DeferedSaveConfig();
			ShowAdjusterToast(retprintf("Zoom Scale: %1.3f", this->stereoView->ZoomOutScale), 500);
		}
		else if(config.HotkeyDistortionScaleMinus->IsPressed(controls) && HotkeysActive())
		{
			this->stereoView->ZoomOutScale = 0.50f;
			this->stereoView->PostReset();	

			DeferedSaveConfig();
			ShowAdjusterToast(retprintf("Zoom Scale: %1.3f", this->stereoView->ZoomOutScale), 500);
		}


		// screenshot - <RCONTROL>+<*>
		if(config.HotkeyScreenshot->IsPressed(controls) && HotkeysActive())
		{
			// render 3 frames to get screenshots
			screenshot = 3;
		}
	
		//Telescopic mode - use ALT + Mouse Wheel CLick
		if (config.HotkeyTelescopeMode->IsPressed(controls) && HotkeysActive())
		{
			//First check whether VRBoost is controlling FOV, we can't use this functionality if it isn't
			bool canUseTelescope = false;
			if (VRBoostStatus.VRBoost_Active)
			{
				ActiveAxisInfo axes[30];
				memset(axes, 0xFF, sizeof(ActiveAxisInfo) * 30);
				UINT count = m_pVRboost_GetActiveRuleAxes((ActiveAxisInfo**)&axes);

				UINT i = 0;
				while (i < count)
				{
					if (axes[i].Axis == MAXDWORD)
						break;
					if (axes[i].Axis == VRboostAxis::WorldFOV)
					{
						canUseTelescope = true;
						break;
					}
					i++;
				}	
			}

			if (canUseTelescope)
			{
				if (!m_telescopicSightMode &&
					m_telescopeTargetFOV == FLT_MAX)
				{   
					//enabling - reduce FOV to 20 (will result in zooming in)
					m_telescopeTargetFOV = 20;
					m_telescopeCurrentFOV = config.WorldFOV;
					stereoView->m_vignetteStyle = StereoView::TELESCOPIC_SIGHT;
					m_telescopicSightMode = true;
				}
				else if (m_telescopicSightMode)
				{
					//disabling
					m_telescopicSightMode = false;
					m_telescopeTargetFOV = config.WorldFOV;
					stereoView->m_vignetteStyle = StereoView::NONE;
				}
			}
		}
	}
}

void D3DProxyDevice::HotkeyCooldown(float duration)
{
	hotkeyCooldown += duration;
}

bool D3DProxyDevice::HotkeysActive()
{
	//return hotkeyCooldown == 0.0f && !hotkeyCatch;
	return !hotkeyCatch;
}
