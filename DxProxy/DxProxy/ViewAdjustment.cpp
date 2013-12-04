/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <ViewAdjustment.cpp> and
Class <ViewAdjustment> :
Copyright (C) 2013 Chris Drain

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


#include "ViewAdjustment.h"

/**
* Constructor.
* Sets class constants, identity matrices and a projection matrix.
***/
ViewAdjustment::ViewAdjustment(HMDisplayInfo &displayInfo, float metersToWorldUnits, bool enableRoll) :
	hmdInfo(displayInfo),
	metersToWorldMultiplier(metersToWorldUnits),
	rollEnabled(enableRoll),
	bulletLabyrinth(false)
{
	// TODO : max, min convergence; arbitrary now
	convergence = 0.0f;
	minConvergence = -10.0f;
	maxConvergence = 10.0f;

	ipd = IPD_DEFAULT;

	n = 0.1f;					
	f = 10.0f;
	l = -0.5f;
	r = 0.5f;

	squash = 1.0f;
	gui3DDepth = 0.0f;
	hudDistance = 0.0f;
	hud3DDepth = 0.0f;

	D3DXMatrixIdentity(&matProjection);
	D3DXMatrixIdentity(&matProjectionInv);
	D3DXMatrixIdentity(&projectLeft);
	D3DXMatrixIdentity(&projectRight);
	D3DXMatrixIdentity(&transformLeft);
	D3DXMatrixIdentity(&transformRight);
	D3DXMatrixIdentity(&matViewProjRight);
	D3DXMatrixIdentity(&matViewProjLeft);
	D3DXMatrixIdentity(&matViewProjTransformRight);
	D3DXMatrixIdentity(&matViewProjTransformLeft);
	D3DXMatrixIdentity(&matViewProjTransformRightNoRoll);
	D3DXMatrixIdentity(&matViewProjTransformLeftNoRoll);
	D3DXMatrixIdentity(&matHudLeft);
	D3DXMatrixIdentity(&matHudRight);
	D3DXMatrixIdentity(&matGuiLeft);
	D3DXMatrixIdentity(&matGuiRight);
	D3DXMatrixIdentity(&matGatheredLeft);
	D3DXMatrixIdentity(&matGatheredRight);

	UpdateProjectionMatrices(displayInfo.screenAspectRatio);
	D3DXMatrixIdentity(&rollMatrix);
	D3DXMatrixIdentity(&rollMatrixNegative);
	ComputeViewTransforms();
}

/**
* Empty destructor.
***/
ViewAdjustment::~ViewAdjustment() 
{
}

/**
* Loads game configuration data.
* @param cfg Game configuration to load.
***/
void ViewAdjustment::Load(ProxyHelper::ProxyConfig& cfg) 
{
	rollEnabled = cfg.rollEnabled;
	metersToWorldMultiplier  = cfg.worldScaleFactor;
	convergence = cfg.convergence;
	ipd = cfg.ipd;
	stereoType = cfg.stereo_mode;
}

/**
* Saves game configuration data.
* @param cfg The game configuration to be saved to.
***/
void ViewAdjustment::Save(ProxyHelper::ProxyConfig& cfg) 
{
	cfg.rollEnabled = rollEnabled;
	cfg.convergence = convergence;

	//worldscale and ipd are not normally edited;
	cfg.worldScaleFactor = metersToWorldMultiplier;
	cfg.ipd = ipd;
}

/**
* Updates left and right projection matrices.
* Now, the convergence point is specified in real, physical meters, since the IPD is also specified
* in physical meters. That means, if the game-specific world scale is set correctly, a convergence
* value of 3.0f would mean that the virtual screen, neutral point or convergence point is 3 meters
* ahead of us.
* @param aspectRation The aspect ratio for the projection matrix.
***/
void ViewAdjustment::UpdateProjectionMatrices(float aspectRatio)
{
	t = 0.5f / aspectRatio;
	b = -0.5f / aspectRatio;

	D3DXMatrixPerspectiveOffCenterLH(&matProjection, l, r, b, t, n, f);
	D3DXMatrixInverse(&matProjectionInv, 0, &matProjection);

	// ALL stated in meters here ! screen size = horizontal size
	float nearClippingPlaneDistance = hmdInfo.eyeToScreenDistance; 
	float physicalScreenSizeInMeters = hmdInfo.physicalScreenSize.first / 2; 

	// if not HMD, set values to fullscreen defaults
	if ((stereoType != 26) && // != StereoView::StereoTypes::OCULUS_RIFT
		(stereoType != 27) && // != StereoView::StereoTypes::OCULUS_RIFT_CROPPED
		(stereoType != 25))   // != StereoView::StereoTypes::DIY_RIFT))
	{
		// assumption here :
		// end user is placed 1 meter away from screen
		// end user screen is 1 meter in horizontal size
		nearClippingPlaneDistance = 1;
		physicalScreenSizeInMeters = 1;
	}

	// convergence frustum adjustment, based on NVidia explanations
	//
	// It is evident that the ratio of frustum shift to the near clipping plane is equal to the ratio of 
	// IOD/2 to the distance from the screenplane. (IOD=IPD) 
	// frustumAsymmetryInMeters = ((IPD/2) * nearClippingPlaneDistance) / convergence
	// <http://www.orthostereo.com/geometryopengl.html>
	//
	// (near clipping plane distance = physical screen distance)
	// (convergence = virtual screen distance)
	if (convergence <= nearClippingPlaneDistance) convergence = nearClippingPlaneDistance + 0.001f;
	float frustumAsymmetryInMeters = ((ipd/2) * nearClippingPlaneDistance) / convergence;

	// divide the frustum asymmetry by the assumed physical size of the physical screen
	float frustumAsymmetryLeftInMeters = (frustumAsymmetryInMeters * LEFT_CONSTANT) / physicalScreenSizeInMeters;
	float frustumAsymmetryRightInMeters = (frustumAsymmetryInMeters * RIGHT_CONSTANT) / physicalScreenSizeInMeters;

	// get the horizontal screen space size and compute screen space adjustment
	float screenSpaceXSize = abs(l)+abs(r);
	float multiplier = screenSpaceXSize/1; // = 1 meter
	float frustumAsymmetryLeft = frustumAsymmetryLeftInMeters * multiplier;
	float frustumAsymmetryRight = frustumAsymmetryRightInMeters * multiplier;

	// now, create the re-projection matrices for both eyes using this frustum asymmetry
	D3DXMatrixPerspectiveOffCenterLH(&projectLeft, l+frustumAsymmetryLeft, r+frustumAsymmetryLeft, b, t, n, f);
	D3DXMatrixPerspectiveOffCenterLH(&projectRight, l+frustumAsymmetryRight, r+frustumAsymmetryRight, b, t, n, f);
}

/**
* Updates the current pitch and yaw head movement.
***/
void ViewAdjustment::UpdatePitchYaw(float pitch, float yaw)
{
	// bullet labyrinth matrix
	D3DXMatrixTranslation(&matBulletLabyrinth, -yaw, pitch, 0.0f);
}

/**
* Updates the roll matrix, seems to be senseless right now, just calls D3DXMatrixRotationZ().
* @param roll Angle of rotation, in radians.
***/
void ViewAdjustment::UpdateRoll(float roll)
{
	D3DXMatrixIdentity(&rollMatrix);
	D3DXMatrixRotationZ(&rollMatrix, roll);
	D3DXMatrixRotationZ(&rollMatrixNegative, -roll);
	D3DXMatrixRotationZ(&rollMatrixHalf, roll * 0.5f);
}

/**
* Gets the view-projection transform matrices left and right.
* Unprojects, shifts view position left/right (using same matricies as (Left/Right)ViewRollAndShift)
* and reprojects using left/right projection.
* (matrix = projectionInverse * transform * projection)
***/
void ViewAdjustment::ComputeViewTransforms()
{
	// separation settings are overall (HMD and desktop), since they are based on physical IPD
	D3DXMatrixTranslation(&transformLeft, SeparationInWorldUnits() * LEFT_CONSTANT, 0, 0);
	D3DXMatrixTranslation(&transformRight, SeparationInWorldUnits() * RIGHT_CONSTANT, 0, 0);

	// projection transform, no roll
	matViewProjTransformLeftNoRoll = matProjectionInv * transformLeft * projectLeft;
	matViewProjTransformRightNoRoll = matProjectionInv * transformRight * projectRight;

	// head roll
	if (rollEnabled) {
		D3DXMatrixMultiply(&transformLeft, &rollMatrix, &transformLeft);
		D3DXMatrixMultiply(&transformRight, &rollMatrix, &transformRight);

		// projection 
		matViewProjLeft = matProjectionInv * rollMatrix * projectLeft;
		matViewProjRight = matProjectionInv * rollMatrix * projectRight;
	}
	else
	{
		// projection 
		matViewProjLeft = matProjectionInv * projectLeft;
		matViewProjRight = matProjectionInv * projectRight;
	}

	// projection transform
	matViewProjTransformLeft = matProjectionInv * transformLeft * projectLeft;
	matViewProjTransformRight = matProjectionInv * transformRight * projectRight;

	// now, create HUD/GUI helper matrices

	// if not HMD, set HUD/GUI to fullscreen
	if ((stereoType != 26) && // != StereoView::StereoTypes::OCULUS_RIFT
		(stereoType != 27) && // != StereoView::StereoTypes::OCULUS_RIFT_CROPPED
		(stereoType != 25))   // != StereoView::StereoTypes::DIY_RIFT))
	{
		squash = 1.0f;
		gui3DDepth = 0.0f;
		hudDistance = 0.0f;
		hud3DDepth = 0.0f;
	}

	// squash
	D3DXMatrixScaling(&matSquash, squash, squash, 1);

	// hudDistance
	D3DXMatrixTranslation(&matHudDistance, 0, 0, hudDistance);

	// hud3DDepth
	D3DXMatrixTranslation(&matLeftHud3DDepth, hud3DDepth, 0, 0);
	D3DXMatrixTranslation(&matRightHud3DDepth, -hud3DDepth, 0, 0);
	float additionalSeparation = (1.5f-hudDistance)*hmdInfo.lensXCenterOffset;
	D3DXMatrixTranslation(&matLeftHud3DDepthShifted, hud3DDepth+additionalSeparation, 0, 0);
	D3DXMatrixTranslation(&matRightHud3DDepthShifted, -hud3DDepth-additionalSeparation, 0, 0);
	D3DXMatrixTranslation(&matLeftGui3DDepth, gui3DDepth+SeparationIPDAdjustment(), 0, 0);
	D3DXMatrixTranslation(&matRightGui3DDepth, -(gui3DDepth+SeparationIPDAdjustment()), 0, 0);

	// gui/hud matrices
	matHudLeft = matProjectionInv * matLeftHud3DDepth * transformLeft * matHudDistance *  projectLeft;
	matHudRight = matProjectionInv * matRightHud3DDepth * transformRight * matHudDistance * projectRight;
	matGuiLeft =  matProjectionInv * matLeftGui3DDepth * matSquash * projectLeft;
	matGuiRight = matProjectionInv * matRightGui3DDepth * matSquash * projectRight;
}

/**
* Returns the left view projection transform matrix.
***/
D3DXMATRIX ViewAdjustment::LeftAdjustmentMatrix()
{
	return matViewProjTransformLeft;
}

/**
* Returns the right view projection transform matrix.
***/
D3DXMATRIX ViewAdjustment::RightAdjustmentMatrix()
{
	return matViewProjTransformRight;
}

/**
* Returns the left view projection transform matrix without head roll.
***/
D3DXMATRIX ViewAdjustment::LeftAdjustmentMatrixNoRoll()
{
	return matViewProjTransformLeftNoRoll;
}

/**
* Returns the right view projection transform matrix without head roll.
***/
D3DXMATRIX ViewAdjustment::RightAdjustmentMatrixNoRoll()
{
	return matViewProjTransformRightNoRoll;
}

/**
* Returns the left matrix used to roll (if roll enabled) and shift view for ipd (without transform).
***/
D3DXMATRIX ViewAdjustment::LeftView()
{
	return matViewProjLeft;
}

/**
* Returns the right matrix used to roll (if roll enabled) and shift view for ipd (without transform).
***/
D3DXMATRIX ViewAdjustment::RightView()
{
	return matViewProjRight;
}

/**
* Returns the left matrix used to roll (if roll enabled) and shift view for ipd.
***/
D3DXMATRIX ViewAdjustment::LeftViewTransform()
{
	return transformLeft;
}

/**
* Returns the right matrix used to roll (if roll enabled) and shift view for ipd.
***/
D3DXMATRIX ViewAdjustment::RightViewTransform()
{
	return transformRight;
}

/**
* Return the current projection matrix.
***/
D3DXMATRIX ViewAdjustment::Projection()
{
	return matProjection;
}

/**
* Returns the current projection inverse matrix.
***/
D3DXMATRIX ViewAdjustment::ProjectionInverse()
{
	return matProjectionInv;
}

/**
* Returns the current roll matrix.
***/
D3DXMATRIX ViewAdjustment::RollMatrix()
{
	return rollMatrix;
}

/**
* Returns the current roll matrix.
***/
D3DXMATRIX ViewAdjustment::RollMatrixNegative()
{
	return rollMatrixNegative;
}

/**
* Returns the current roll matrix. (half roll)
***/
D3DXMATRIX ViewAdjustment::RollMatrixHalf()
{
	return rollMatrixHalf;
}

/**
* Returns the current right GUI matrix.
***/
D3DXMATRIX ViewAdjustment::LeftHUDMatrix()
{
	return matHudLeft;
}

/**
* Returns the current right GUI matrix.
***/
D3DXMATRIX ViewAdjustment::RightHUDMatrix()
{
	return matHudRight;
}

/**
* Returns the current right GUI matrix.
***/
D3DXMATRIX ViewAdjustment::LeftGUIMatrix()
{
	return matGuiLeft;
}

/**
* Returns the current right GUI matrix.
***/
D3DXMATRIX ViewAdjustment::RightGUIMatrix()
{
	return matGuiRight;
}

/**
* Returns the current projection inverse matrix.
***/
D3DXMATRIX ViewAdjustment::Squash()
{
	return matSquash;
}

/**
* Returns the current projection inverse matrix.
***/
D3DXMATRIX ViewAdjustment::HUDDistance()
{
	return matHudDistance;
}

/**
* Returns the current left HUD depth eye separation matrix.
***/
D3DXMATRIX ViewAdjustment::LeftHUD3DDepth()
{
	return matLeftHud3DDepth;
}

/**
* Returns the current left HUD depth eye separation matrix.
***/
D3DXMATRIX ViewAdjustment::RightHUD3DDepth()
{
	return matRightHud3DDepth;
}

/**
* Returns the current left HUD depth eye separation matrix shifted.
***/
D3DXMATRIX ViewAdjustment::LeftHUD3DDepthShifted()
{
	return matLeftHud3DDepthShifted;
}

/**
* Returns the current left HUD depth eye separation matrix shifted.
***/
D3DXMATRIX ViewAdjustment::RightHUD3DDepthShifted()
{
	return matRightHud3DDepthShifted;
}

/**
* Returns the current left HUD depth eye separation matrix.
***/
D3DXMATRIX ViewAdjustment::LeftGUI3DDepth()
{
	return matLeftGui3DDepth;
}

/**
* Returns the current left HUD depth eye separation matrix.
***/
D3DXMATRIX ViewAdjustment::RightGUI3DDepth()
{
	return matRightGui3DDepth;
}

/**
* Returns the current bullet labyrinth matrix.
***/
D3DXMATRIX ViewAdjustment::BulletLabyrinth()
{
	return matBulletLabyrinth;
}

/**
* Returns the current left gathered matrix.
***/
D3DXMATRIX ViewAdjustment::GatheredMatrixLeft()
{
	return matGatheredLeft;
}

/**
* Returns the current right gathered matrix.
***/
D3DXMATRIX ViewAdjustment::GatheredMatrixRight()
{
	return matGatheredRight;
}

/**
* Gathers a matrix to be used in modifications.
***/
void ViewAdjustment::GatherMatrix(D3DXMATRIX& matrixLeft, D3DXMATRIX& matrixRight)
{
	matGatheredLeft = D3DXMATRIX(matrixLeft);
	matGatheredRight = D3DMATRIX(matrixRight);
}

/**
* Returns the current world scale.
***/
float ViewAdjustment::WorldScale()
{
	return metersToWorldMultiplier;
}

/**
* Modifies the world scale with its limits 0.000001f and 1,000,000 (arbitrary limit).
* NOTE: This should not be changed during normal usage, this is here to facilitate finding a reasonable scale.
***/
float ViewAdjustment::ChangeWorldScale(float toAdd)
{
	metersToWorldMultiplier+= toAdd;

	vireio::clamp(&metersToWorldMultiplier, 0.000001f, 1000000.0f);

	return metersToWorldMultiplier;
}

/**
* Changes and clamps convergence.
***/
float ViewAdjustment::ChangeConvergence(float toAdd)
{
	convergence += toAdd;

	vireio::clamp(&convergence, minConvergence, maxConvergence);

	return convergence;
}

/**
* Changes GUI squash and updates matrix.
***/
void ViewAdjustment::ChangeGUISquash(float newSquash)
{
	squash = newSquash;

	D3DXMatrixScaling(&matSquash, squash, squash, 1);
}

/**
* Changes the GUI eye separation (=GUI 3D Depth) and updates matrices.
***/
void ViewAdjustment::ChangeGUI3DDepth(float newGui3DDepth)
{
	gui3DDepth = newGui3DDepth;

	D3DXMatrixTranslation(&matLeftGui3DDepth, gui3DDepth+SeparationIPDAdjustment(), 0, 0);
	D3DXMatrixTranslation(&matRightGui3DDepth, -(gui3DDepth+SeparationIPDAdjustment()), 0, 0);
}

/**
* Changes the distance of the HUD and updates matrix.
***/
void ViewAdjustment::ChangeHUDDistance(float newHudDistance)
{
	hudDistance = newHudDistance;

	D3DXMatrixTranslation(&matHudDistance, 0, 0, hudDistance);
}

/**
*  Changes the HUD eye separation (=HUD 3D Depth) and updates matrices.
***/
void ViewAdjustment::ChangeHUD3DDepth(float newHud3DDepth)
{
	hud3DDepth = newHud3DDepth;

	D3DXMatrixTranslation(&matLeftHud3DDepth, -hud3DDepth, 0, 0);
	D3DXMatrixTranslation(&matRightHud3DDepth, hud3DDepth, 0, 0);
	float additionalSeparation = (1.5f-hudDistance)*hmdInfo.lensXCenterOffset;
	D3DXMatrixTranslation(&matLeftHud3DDepthShifted, hud3DDepth+additionalSeparation, 0, 0);
	D3DXMatrixTranslation(&matRightHud3DDepthShifted, -hud3DDepth-additionalSeparation, 0, 0);
}

/**
* Set to true if orthographical matrices should be rotated in a bullet labyrinth style.
***/
void ViewAdjustment::SetBulletLabyrinthMode(bool newMode)
{
	bulletLabyrinth = newMode;
}

/**
* True if bullet-labyrinth mode is on.
***/
bool ViewAdjustment::BulletLabyrinthMode()
{
	return bulletLabyrinth;
}

/**
* Just sets world scale to 3.0f.
***/
void ViewAdjustment::ResetWorldScale()
{
	metersToWorldMultiplier = 3.0f;
}

/**
* Just sets convergence to 3.0f (= 3 physical meters).
***/
void ViewAdjustment::ResetConvergence()
{
	convergence = 3.0f;
}

/**
* Returns the current convergence adjustment, in meters.
***/
float ViewAdjustment::Convergence() 
{ 
	return convergence; 
}

/**
* Returns the current convergence adjustment, in game units.
***/
float ViewAdjustment::ConvergenceInWorldUnits() 
{ 
	return convergence * metersToWorldMultiplier; 
}

/**
* Returns the separation being used for view adjustments in game units.
***/
float ViewAdjustment::SeparationInWorldUnits() 
{ 
	return  (ipd / 2.0f) * metersToWorldMultiplier; 
}

/**
* Returns the separation IPD adjustment being used for GUI and HUD matrices.
* (or whenever the eye separation is set manually)
***/
float ViewAdjustment::SeparationIPDAdjustment() 
{ 
	return  ((ipd-IPD_DEFAULT) / 2.0f) * metersToWorldMultiplier; 
}

/**
* Returns true if head roll is enabled.
***/
bool ViewAdjustment::RollEnabled() 
{ 
	return rollEnabled; 
}

/**
* Returns the head mounted display info.
***/
HMDisplayInfo ViewAdjustment::HMDInfo()
{
	return hmdInfo;
}