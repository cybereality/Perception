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


ViewAdjustment::ViewAdjustment(HMDisplayInfo &displayInfo, float metersToWorldUnits, bool enableRoll) :
	hmdInfo(displayInfo),
	metersToWorldMultiplier(metersToWorldUnits),
	rollEnabled(enableRoll)
{
	separationAdjustment = 0.0f;

	minSeparationAdjusment = -SEPARATION_DEFAULT;
	maxSeparationAdjusment = 4 * SEPARATION_DEFAULT;

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
	D3DXMatrixIdentity(&matViewProjTransformRight);
	D3DXMatrixIdentity(&matViewProjTransformLeft);

	UpdateProjectionMatrices(displayInfo.screenAspectRatio);
	D3DXMatrixIdentity(&rollMatrix);
	ComputeViewTransforms();
}

ViewAdjustment::~ViewAdjustment() 
{
}


void ViewAdjustment::UpdateProjectionMatrices(float aspectRatio)
{
	t = 0.5f / aspectRatio;
	b = -0.5f / aspectRatio;

	D3DXMatrixPerspectiveOffCenterLH(&matProjection, l, r, b, t, n, f);
	D3DXMatrixInverse(&matProjectionInv, 0, &matProjection);


	// Based on Rift docs way. //TODO still need to use the 'old way' (or something similar/different) for rendering on a monitor? needs testing
	// if (HMD)
	// Shift projection centers to the center of each lens (assumption: lenses are vertically centered)
	float viewCenter = hmdInfo.physicalScreenSize.first * 0.25f;
	float eyeProjectionShift = viewCenter - hmdInfo.physicaLensSeparation * 0.5f;
	float projectionCenterOffset = 4.0f * eyeProjectionShift / hmdInfo.physicalScreenSize.first;

	D3DXMatrixTranslation(&leftShiftProjection, projectionCenterOffset * LEFT_CONSTANT, 0, 0);
	D3DXMatrixTranslation(&rightShiftProjection, projectionCenterOffset * RIGHT_CONSTANT, 0, 0);
	// else if desktop screen {}

	// old way
	//float adjustedFrustumOffsetLeft = convergence * LEFT_CONSTANT * 0.1f * separation;
	//float adjustedFrustumOffsetRight = convergence * RIGHT_CONSTANT * 0.1f * separation;

	//D3DXMatrixPerspectiveOffCenterLH(&reProjectLeft, l+adjustedFrustumOffsetLeft, r+adjustedFrustumOffsetLeft, b, t, n, f);
	//D3DXMatrixPerspectiveOffCenterLH(&reProjectRight, l+adjustedFrustumOffsetRight, r+adjustedFrustumOffsetRight, b, t, n, f);
	
	projectLeft = leftShiftProjection * matProjection;
	projectRight = rightShiftProjection * matProjection;
}


void ViewAdjustment::UpdateRoll(float roll)
{
	D3DXMatrixIdentity(&rollMatrix);
	D3DXMatrixRotationZ(&rollMatrix, roll);
}

// This translation is applied to vertex shader matricies in various ways by matrix modifications
// Note that l/r frustrum changes are applied differently for the transform and would seem
// to produce different results. So I leave merging this with Transform view/projection code to someone braver.
// But it really feels like it should be a single code path situation.
void ViewAdjustment::ComputeViewTransforms()
{
	//activeSeparation = separation;

	D3DXMATRIX transformLeft;
	D3DXMATRIX transformRight;
	//D3DXMatrixTranslation(&transformLeft, separation * LEFT_CONSTANT * 10.0f, 0, 0);
	//D3DXMatrixTranslation(&transformRight, separation * RIGHT_CONSTANT * 10.0f, 0, 0);

	// if (HMD)
	D3DXMatrixTranslation(&transformLeft, SeparationInWorldUnits() * LEFT_CONSTANT, 0, 0);
	D3DXMatrixTranslation(&transformRight, SeparationInWorldUnits() * RIGHT_CONSTANT, 0, 0);
	// else if desktop screen {}


	D3DXMATRIX rollTransform;
	D3DXMatrixIdentity(&rollTransform);

	if (rollEnabled) {
		D3DXMatrixMultiply(&transformLeft, &rollMatrix, &transformLeft);
		D3DXMatrixMultiply(&transformRight, &rollMatrix, &transformRight);
	}
	

	matViewProjTransformLeft = matProjectionInv * transformLeft * projectLeft;
	matViewProjTransformRight = matProjectionInv * transformRight * projectRight;
}

D3DXMATRIX ViewAdjustment::LeftAdjustmentMatrix()
{
	return matViewProjTransformLeft;
}

D3DXMATRIX ViewAdjustment::RightAdjustmentMatrix()
{
	return matViewProjTransformRight;
}

D3DXMATRIX ViewAdjustment::LeftShiftProjection()
{
	return leftShiftProjection;
}

D3DXMATRIX ViewAdjustment::RightShiftProjection()
{
	return rightShiftProjection;
}

D3DXMATRIX ViewAdjustment::Projection()
{
	return matProjection;
}

D3DXMATRIX ViewAdjustment::ProjectionInverse()
{
	return matProjectionInv;
}

float ViewAdjustment::ChangeSeparationAdjustment(float toAdd)
{
	separationAdjustment += toAdd;
	
	vireio::clamp(&separationAdjustment, minSeparationAdjusment, maxSeparationAdjusment);

	return separationAdjustment;
}

float ViewAdjustment::SeparationInWorldUnits() 
{ 
	return (separationAdjustment + SEPARATION_DEFAULT) * metersToWorldMultiplier; 
}

float ViewAdjustment::SeparationAdjustment() 
{ 
	return separationAdjustment; 
}

bool ViewAdjustment::RollEnabled() 
{ 
	return rollEnabled; 
}

void ViewAdjustment::EnableRoll(bool enable)
{
	rollEnabled = enable;
}

void ViewAdjustment::SetWorldScaleFactor(float scale)
{
	metersToWorldMultiplier = scale;
}