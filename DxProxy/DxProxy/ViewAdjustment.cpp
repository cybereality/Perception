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
#include "D3DProxyDevice.h"
#include "VireioUtil.h"
#include "ShaderConstantmodification.h"
#include "ConfigDefaults.h"
using namespace vireio;

#define PI 3.141592654

// TODO : max, min convergence; arbitrary now
const float minConvergence = -10.0f;
const float maxConvergence = 100.0f;

/**
* Constructor.
* Sets class constants, identity matrices and a projection matrix.
***/
ViewAdjustment::ViewAdjustment(HMDisplayInfo *displayInfo, ProxyConfig *config) :
	config(config),
	hmdInfo(displayInfo),
	m_roll(0.0f)
{
	convergence = 100.0f;

	ipd = IPD_DEFAULT;

	squash = 1.0f;
	gui3DDepth = 0.0f;
	hudDistance = 0.0f;
	hud3DDepth = 0.0f;

	D3DXMatrixIdentity(&matBasicProjection);
	D3DXMatrixIdentity(&matPosition);
	D3DXMatrixIdentity(&matProjectionInv);
	D3DXMatrixIdentity(&gamePerspective);
	D3DXMatrixIdentity(&gamePerspectiveInv);
	D3DXMatrixIdentity(&projectPFOV);
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

	UpdateProjectionMatrices(displayInfo->GetScreenAspectRatio(), 110.0f);
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
void ViewAdjustment::Load(ProxyConfig& cfg) 
{
	convergence = cfg.convergence;
	ipd = cfg.ipd;
}

/**
* Saves game configuration data.
* @param cfg The game configuration to be saved to.
***/
void ViewAdjustment::Save(ProxyConfig& cfg) 
{
	cfg.convergence = convergence;

	//worldscale and ipd are not normally edited;
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
void ViewAdjustment::UpdateProjectionMatrices(float aspectRatio, float fov_horiz)
{
	// Minimum (near) Z-value
	float n = 0.1f;
	// Maximum (far) Z-value
	float f = 10.0f;
	// Minimum (left) X-value
	float l = -0.5f;
	// Maximum (right) X-value
	float r = 0.5f;
	// Maximum (top) y-value of the view volume
	float t = 0.5f / aspectRatio;
	// Minimum (bottom) y-value of the volume
	float b = -0.5f / aspectRatio;

	//Calculate inverse projection
	D3DXMatrixPerspectiveOffCenterLH(&matBasicProjection, l, r, b, t, n, f);
	D3DXMatrixInverse(&matProjectionInv, 0, &matBasicProjection);

	// if not HMD, set values to fullscreen defaults
	if (config->stereo_mode<100 || !config->PFOVToggle)   //stereo type > 100 reserved specifically for HMDs
	{
		// assumption here :
		// end user is placed 1 meter away from screen
		// end user screen is 1 meter in horizontal size
		float nearClippingPlaneDistance = 1;
		float physicalScreenSizeInMeters = 1;

		if (config->stereo_mode<100)
		{
			nearClippingPlaneDistance = hmdInfo->GetEyeToScreenDistance(); 
			physicalScreenSizeInMeters = hmdInfo->GetPhysicalScreenSize().first / 2; 
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
		
		// Convergence being disabled is equivalent to an infinite convergence distance,
		// or zero asymmetry.
		if(!config->convergenceEnabled)
			frustumAsymmetryInMeters = 0.0f;

		// divide the frustum asymmetry by the assumed physical size of the physical screen
		float frustumAsymmetryLeftInMeters = (frustumAsymmetryInMeters * LEFT_CONSTANT) / physicalScreenSizeInMeters;
		float frustumAsymmetryRightInMeters = (frustumAsymmetryInMeters * RIGHT_CONSTANT) / physicalScreenSizeInMeters;

		// get the horizontal screen space size and compute screen space adjustment
		float screenSpaceXSize = abs(l)+abs(r);
		float multiplier = screenSpaceXSize/1; // = 1 meter
		float frustumAsymmetryLeft = frustumAsymmetryLeftInMeters * multiplier;
		float frustumAsymmetryRight = frustumAsymmetryRightInMeters * multiplier;

		// now, create the re-projection matrices for both eyes using this frustum asymmetry
		D3DXMatrixPerspectiveOffCenterLH(&projectLeftConverge, l+frustumAsymmetryLeft, r+frustumAsymmetryLeft, b, t, n, f);
		D3DXMatrixPerspectiveOffCenterLH(&projectRightConverge, l+frustumAsymmetryRight, r+frustumAsymmetryRight, b, t, n, f);
	}
	else
	{
		//Calculate vertical fov from provided horizontal
		float fov_vert = 2.0f * atan( tan(D3DXToRadian(fov_horiz) / 2.0f) * aspectRatio);

		//And left and right (identical in this case)
		D3DXMatrixPerspectiveFovLH(&projectPFOV, fov_vert, aspectRatio, n, f);
	}
	
	ComputeViewTransforms();
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
	m_roll = roll;
}
void ViewAdjustment::SetGameSpecificPositionalScaling(D3DXVECTOR3 scalingVec)
{
	gameScaleVec  = scalingVec;
}

/**
* Updates the position matrix. Angles are in radians.
***/
void ViewAdjustment::UpdatePosition(float yaw, float pitch, float roll, float xPosition, float yPosition, float zPosition)
{
	this->yaw = yaw;
	this->pitch = pitch;
	
	// FIXME: The roll UpdatePosition gets is ignored; roll comes through UpdateRoll.
	//this->roll = roll;
	
	this->xPosition = xPosition;
	this->yPosition = yPosition;
	this->zPosition = zPosition;
	ComputeViewTransforms();
}

void ViewAdjustment::SetPerspective(D3DXMATRIX matrix)
{
	if (!MatrixIsOrtho(matrix))
	{
		gamePerspective = matrix;
		D3DXMatrixInverse(&gamePerspectiveInv, 0, &gamePerspective);
		
		ComputeViewTransforms();
	}
}

/**
* Gets the view-projection transform matrices left and right.
* Unprojects, shifts view position left/right (using same matricies as (Left/Right)ViewRollAndShift)
* and reprojects using left/right projection.
* (matrix = projectionInverse * transform * projection)
***/
void ViewAdjustment::ComputeViewTransforms()
{
	D3DXMATRIX rotationMatrixPitch;
	D3DXMATRIX rotationMatrixYaw;
	D3DXMATRIX rotationMatrixRoll;

	D3DXMatrixRotationX(&rotationMatrixPitch, pitch);
	D3DXMatrixRotationY(&rotationMatrixYaw, yaw);
	D3DXMatrixRotationZ(&rotationMatrixRoll, -roll);

	//Need to invert X and Y
	D3DXVECTOR3 vec(xPosition, yPosition, zPosition);

	D3DXMATRIX worldScale;
	float scaler = config->position_multiplier * config->worldScaleFactor;
	D3DXMatrixScaling(&worldScale, -1.0f*scaler, -1.0f*scaler, scaler);
	D3DXVec3TransformNormal(&positionTransformVec, &vec, &worldScale);

	D3DXMATRIX rotationMatrixPitchYaw;
	D3DXMatrixIdentity(&rotationMatrixPitchYaw);
	D3DXMatrixMultiply(&rotationMatrixPitchYaw, &rotationMatrixYaw, &rotationMatrixPitch);

	D3DXVec3TransformNormal(&positionTransformVec, &positionTransformVec, &rotationMatrixPitchYaw);

	//Still need to apply the roll, as the "no roll" param is just whether we use matrix roll translation or if
	//memory modification, either way, the view still rolls, unless using the pixel shader roll approach
	if (config->rollImpl != 2)
	{
		D3DXVec3TransformNormal(&positionTransformVec, &positionTransformVec, &rotationMatrixRoll);
	}

	//Now apply game specific scaling for the X/Y/Z
	D3DXMATRIX gamescalingmatrix;
	D3DXMatrixScaling(&gamescalingmatrix, config->position_x_multiplier, config->position_y_multiplier, config->position_z_multiplier);
	D3DXVec3TransformNormal(&positionTransformVec, &positionTransformVec, &gamescalingmatrix);
	
	D3DXMatrixTranslation(&matPosition, positionTransformVec.x, positionTransformVec.y, positionTransformVec.z);
	
	// Make screen-space position correction matrix with game perspective matrix
	matScreenSpacePositionLeft = gamePerspectiveInv * matPosition * gamePerspective;
	matScreenSpacePositionRight = gamePerspectiveInv * matPosition * gamePerspective;
	
	//Pixel shader roll needs to apply stereo separation adjusted for the tilted horizon
	if (config->rollImpl == 2)
	{
		float xLeftSeparation = cos(-m_roll) * SeparationInWorldUnits() * LEFT_CONSTANT;
		float yLeftSeparation = sin(-m_roll) * SeparationInWorldUnits() * LEFT_CONSTANT;
		float xRightSeparation = cos(-m_roll) * SeparationInWorldUnits() * RIGHT_CONSTANT;
		float yRightSeparation = sin(-m_roll) * SeparationInWorldUnits() * RIGHT_CONSTANT;

		// separation settings are overall (HMD and desktop), since they are based on physical IPD
		D3DXMatrixTranslation(&transformLeft, xLeftSeparation, yLeftSeparation, 0);
		D3DXMatrixTranslation(&transformRight, xRightSeparation, yRightSeparation, 0);
	}
	else
	{
		// separation settings are overall (HMD and desktop), since they are based on physical IPD
		D3DXMatrixTranslation(&transformLeft, SeparationInWorldUnits() * LEFT_CONSTANT, 0, 0);
		D3DXMatrixTranslation(&transformRight, SeparationInWorldUnits() * RIGHT_CONSTANT, 0, 0);
	}
	
	// optionalRollMatrix: Either a rotation matrix that applies roll, or the identity
	// matrix, depending on config->rollImpl.
	D3DXMATRIX optionalRollMatrix;
	if (config->rollImpl == 1) {
		optionalRollMatrix = rollMatrix;
	} else {
		D3DXMatrixIdentity(&optionalRollMatrix);
	}
	
	D3DXMATRIX projectLeftOrPFOV  = m_usePFOV ? projectPFOV : projectLeftConverge;
	D3DXMATRIX projectRightOrPFOV = m_usePFOV ? projectPFOV : projectRightConverge;
	
	// projection transform, no roll
	matViewProjTransformLeftNoRoll = matProjectionInv * transformLeft * projectLeftOrPFOV;
	matViewProjTransformRightNoRoll = matProjectionInv * transformRight * projectRightOrPFOV;

	// head roll - only if using translation implementation
	transformLeft = optionalRollMatrix * transformLeft;
	transformRight = optionalRollMatrix * transformRight;
	
	// projection
	matViewProjLeft = matProjectionInv * optionalRollMatrix * projectLeftOrPFOV;
	matViewProjRight = matProjectionInv * optionalRollMatrix * projectRightOrPFOV;

	// projection transform
	matViewProjTransformLeft = matProjectionInv * transformLeft * projectLeftOrPFOV;
	matViewProjTransformRight = matProjectionInv * transformRight * projectRightOrPFOV;

	// now, create HUD/GUI helper matrices

	// if not HMD, set HUD/GUI to fullscreen
	if (config->stereo_mode <100)   //stereo type > 100 reserved specifically for HMDs
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
	float additionalSeparation = (1.5f-hudDistance)*hmdInfo->GetLensXCenterOffset();
	D3DXMatrixTranslation(&matLeftHud3DDepthShifted, hud3DDepth+additionalSeparation, 0, 0);
	D3DXMatrixTranslation(&matRightHud3DDepthShifted, -hud3DDepth-additionalSeparation, 0, 0);
	D3DXMatrixTranslation(&matLeftGui3DDepth, gui3DDepth+SeparationIPDAdjustment(), 0, 0);
	D3DXMatrixTranslation(&matRightGui3DDepth, -(gui3DDepth+SeparationIPDAdjustment()), 0, 0);

	// gui/hud matrices - Just use the default projection not the PFOV
	matHudLeft = matProjectionInv * matLeftHud3DDepth * transformLeft * matHudDistance *  matBasicProjection;
	matHudRight = matProjectionInv * matRightHud3DDepth * transformRight * matHudDistance * matBasicProjection;
	matGuiLeft =  matProjectionInv * matLeftGui3DDepth * matSquash * matBasicProjection;
	matGuiRight = matProjectionInv * matRightGui3DDepth * matSquash * matBasicProjection;
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
* Modifies the world scale with its limits 0.000001f and 1,000,000 (arbitrary limit).
* NOTE: This should not be changed during normal usage, this is here to facilitate finding a reasonable scale.
***/
float ViewAdjustment::ChangeWorldScale(float toAdd)
{
	config->worldScaleFactor+= toAdd;
	vireio::clamp(&config->worldScaleFactor, 0.000001f, 1000000.0f);
	return config->worldScaleFactor;
}

float ViewAdjustment::SetConvergence(float newConvergence)
{
	this->convergence = newConvergence;
	return newConvergence;
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
	float additionalSeparation = (1.5f-hudDistance)*hmdInfo->GetLensXCenterOffset();
	D3DXMatrixTranslation(&matLeftHud3DDepthShifted, hud3DDepth+additionalSeparation, 0, 0);
	D3DXMatrixTranslation(&matRightHud3DDepthShifted, -hud3DDepth-additionalSeparation, 0, 0);
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
	return convergence * config->worldScaleFactor;
}

/**
* Returns the separation being used for view adjustments in game units.
***/
float ViewAdjustment::SeparationInWorldUnits()
{ 
	return  (ipd / 2.0f) * config->worldScaleFactor;
}

/**
* Returns the separation IPD adjustment being used for GUI and HUD matrices.
* (or whenever the eye separation is set manually)
***/
float ViewAdjustment::SeparationIPDAdjustment()
{
	return  ((ipd-IPD_DEFAULT) / 2.0f) * config->worldScaleFactor;
}

/**
* Returns the head mounted display info.
***/
HMDisplayInfo* ViewAdjustment::HMDInfo()
{
	return hmdInfo;
}
