/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2013 Chris Drain

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
	rollEnabled(enableRoll)
{
	separationAdjustment = 0.0f;
	minSeperationAdjustment = -(IPD_DEFAULT / 2.0f);
	maxSeparationAdjustment = 4 * (IPD_DEFAULT / 2.0f);

	ipd = IPD_DEFAULT;

	n = 0.1f;					
	f = 10.0f;
	l = -0.5f;
	r = 0.5f;

	D3DXMatrixIdentity(&matProjection);
	D3DXMatrixIdentity(&matProjectionInv);
	D3DXMatrixIdentity(&leftShiftProjection);
	D3DXMatrixIdentity(&rightShiftProjection);
	D3DXMatrixIdentity(&projectLeft);
	D3DXMatrixIdentity(&projectRight);
	D3DXMatrixIdentity(&transformLeft);
	D3DXMatrixIdentity(&transformRight);
	D3DXMatrixIdentity(&matViewProjTransformRight);
	D3DXMatrixIdentity(&matViewProjTransformLeft);

	UpdateProjectionMatrices(displayInfo.screenAspectRatio);
	D3DXMatrixIdentity(&rollMatrix);
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
	separationAdjustment = cfg.separationAdjustment;
	ipd = cfg.ipd;
}

/**
* Saves game configuration data.
* @param cfg The game configuration to be saved to.
***/
void ViewAdjustment::Save(ProxyHelper::ProxyConfig& cfg) 
{
	cfg.rollEnabled = rollEnabled;
	cfg.separationAdjustment = separationAdjustment;

	//worldscale and ipd are not normally edited;
	cfg.worldScaleFactor = metersToWorldMultiplier;
	cfg.ipd = ipd;
}

/**
* Updates left and right projection matrices. (much to do here!!)
* @param aspectRation The aspect ratio for the projection matrix.
***/
void ViewAdjustment::UpdateProjectionMatrices(float aspectRatio)
{
	t = 0.5f / aspectRatio;
	b = -0.5f / aspectRatio;

	D3DXMatrixPerspectiveOffCenterLH(&matProjection, l, r, b, t, n, f);
	D3DXMatrixInverse(&matProjectionInv, 0, &matProjection);

	// Based on Rift docs way. //TODO still need to use the 'old way' (or something similar/different) for rendering on a monitor? needs testing
	// if (HMD)
	// The lensXCenterOffset is in the same -1 to 1 space as the perspective so shift by that amount to move projection in line with the lenses
	D3DXMatrixTranslation(&leftShiftProjection, hmdInfo.lensXCenterOffset * LEFT_CONSTANT, 0, 0);
	D3DXMatrixTranslation(&rightShiftProjection, hmdInfo.lensXCenterOffset * RIGHT_CONSTANT, 0, 0);
	// else if desktop screen {}

	// old way
	//float adjustedFrustumOffsetLeft = convergence * LEFT_CONSTANT * 0.1f * separation;
	//float adjustedFrustumOffsetRight = convergence * RIGHT_CONSTANT * 0.1f * separation;

	//D3DXMatrixPerspectiveOffCenterLH(&reProjectLeft, l+adjustedFrustumOffsetLeft, r+adjustedFrustumOffsetLeft, b, t, n, f);
	//D3DXMatrixPerspectiveOffCenterLH(&reProjectRight, l+adjustedFrustumOffsetRight, r+adjustedFrustumOffsetRight, b, t, n, f);

	projectLeft = matProjection * leftShiftProjection;
	projectRight = matProjection * rightShiftProjection;
}

/**
* Updates the roll matrix, seems to be senseless right now, just calls D3DXMatrixRotationZ().
* @param roll Angle of rotation, in radians.
***/
void ViewAdjustment::UpdateRoll(float roll)
{
	D3DXMatrixIdentity(&rollMatrix);
	D3DXMatrixRotationZ(&rollMatrix, roll);
}

/**
* Gets the view-projection transform matrices left and right.
* Unprojects, shifts view position left/right (using same matricies as (Left/Right)ViewRollAndShift)
* and reprojects using left/right projection.
* (matrix = projectionInverse * transform * projection)
***/
void ViewAdjustment::ComputeViewTransforms()
{
	// if (HMD)
	D3DXMatrixTranslation(&transformLeft, SeparationInWorldUnits() * LEFT_CONSTANT, 0, 0);
	D3DXMatrixTranslation(&transformRight, SeparationInWorldUnits() * RIGHT_CONSTANT, 0, 0);
	// else if desktop screen {}

	//D3DXMATRIX rollTransform;
	//D3DXMatrixIdentity(&rollTransform);

	if (rollEnabled) {
		D3DXMatrixMultiply(&transformLeft, &rollMatrix, &transformLeft);
		D3DXMatrixMultiply(&transformRight, &rollMatrix, &transformRight);
	}

	matViewProjTransformLeft = matProjectionInv * transformLeft * projectLeft;
	matViewProjTransformRight = matProjectionInv * transformRight * projectRight;
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
* Returns the left shifted projection.
* (projection * This shift = left/right shifted projection)
***/
D3DXMATRIX ViewAdjustment::LeftShiftProjection()
{
	return leftShiftProjection;
}

/**
* Returns the right shifted projection.
* (projection * This shift = left/right shifted projection) 
***/
D3DXMATRIX ViewAdjustment::RightShiftProjection()
{
	return rightShiftProjection;
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
* Modifies the world scale with its limits 0.01f and 1,000,000 (arbitrary limit).
* NOTE: This should not be changed during normal usage, this is here to facilitate finding a reasonable scale.
***/
float ViewAdjustment::ChangeWorldScale(float toAdd)
{
	metersToWorldMultiplier+= toAdd;

	vireio::clamp(&metersToWorldMultiplier, 0.01f, 1000000.0f);

	return metersToWorldMultiplier;
}

/**
* Returns the new separation adjustment in m. 
* (toAdd is the amount to adjust the separation by in m)
* new adjustment might be the same as old adjustment if adjustment limit is reached. 
***/
float ViewAdjustment::ChangeSeparationAdjustment(float toAdd)
{
	separationAdjustment += toAdd;

	vireio::clamp(&separationAdjustment, minSeperationAdjustment, maxSeparationAdjustment);

	return separationAdjustment;
}

/**
* Currently just sets seperation adjustment to zero.
***/
void ViewAdjustment::ResetSeparationAdjustment()
{
	separationAdjustment = 0.0f;
}

/**
* Returns the current separation adjustment being used in m (this is game and user specific and should be saved appropriately).
* TODO remove this and set on gamehandler which has a 'current user'? 
***/
float ViewAdjustment::SeparationAdjustment() 
{ 
	return separationAdjustment; 
}

/**
* Returns the separation being used for view adjustments in game units.
***/
float ViewAdjustment::SeparationInWorldUnits() 
{ 
	return (separationAdjustment + (IPD_DEFAULT / 2.0f)) * metersToWorldMultiplier; 
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