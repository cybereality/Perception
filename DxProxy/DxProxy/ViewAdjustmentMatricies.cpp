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


#include "ViewAdjustmentMatricies.h"


ViewAdjustmentMatricies::ViewAdjustmentMatricies()
{
	n = 0.1f;					
	f = 10.0f;
	l = -0.5f;
	r = 0.5f;

	UpdateProjectionMatrices(0, 0, 1.6f);
	D3DXMatrixIdentity(&rollMatrix);
	ComputeViewTranslations(0, 0, false);
}

ViewAdjustmentMatricies::~ViewAdjustmentMatricies() 
{
}


void ViewAdjustmentMatricies::UpdateProjectionMatrices(float separation, float convergence, float aspectRatio)
{	// call this function after StereoView::Init()

	//aspectRatio = (float)stereoView->viewport.Width/(float)stereoView->viewport.Height;
	t = 0.5f / aspectRatio;
	b = -0.5f / aspectRatio;

	D3DXMatrixPerspectiveOffCenterLH(&matProjection, l, r, b, t, n, f);
	D3DXMatrixInverse(&matProjectionInv, 0, &matProjection);

	float adjustedFrustumOffsetLeft = convergence * LEFT_CONSTANT * 0.1f * separation;		
	float adjustedFrustumOffsetRight = convergence * RIGHT_CONSTANT * 0.1f * separation;		

	D3DXMATRIX reProjectLeft;
	D3DXMATRIX reProjectRight;
	D3DXMatrixPerspectiveOffCenterLH(&reProjectLeft, l+adjustedFrustumOffsetLeft, r+adjustedFrustumOffsetLeft, b, t, n, f);
	D3DXMatrixPerspectiveOffCenterLH(&reProjectRight, l+adjustedFrustumOffsetRight, r+adjustedFrustumOffsetRight, b, t, n, f);
}


void ViewAdjustmentMatricies::UpdateRoll(float roll)
{
	D3DXMatrixIdentity(&rollMatrix);
	D3DXMatrixRotationZ(&rollMatrix, roll);
}

// This translation is applied to vertex shader matricies in some subclasses.
// Note that l/r frustrum changes are applied differently for the transform and would seem
// to produce different results. So I leave merging this with Transform view/projection code to someone braver.
// But it really feels like it should be a single code path situation.
void ViewAdjustmentMatricies::ComputeViewTranslations(float separation, float convergence, bool rollEnabled)
{
	D3DXMATRIX transformLeft;
	D3DXMATRIX transformRight;
	D3DXMatrixTranslation(&transformLeft, separation * LEFT_CONSTANT * 10.0f, 0, 0);
	D3DXMatrixTranslation(&transformRight, separation * RIGHT_CONSTANT * 10.0f, 0, 0);

	D3DXMATRIX rollTransform;
	D3DXMatrixIdentity(&rollTransform);

	if (rollEnabled) {
		D3DXMatrixMultiply(&transformLeft, &rollMatrix, &transformLeft);
		D3DXMatrixMultiply(&transformRight, &rollMatrix, &transformRight);
	}
	


	// TODO this part only needs recalculating after a 3d settings change /////////////////
	
	///////////////////////////////////////////////////////////////////////////////////////
	

	matViewProjTranslateLeft = matProjectionInv * transformLeft * reProjectLeft;
	matViewProjTranslateRight = matProjectionInv * transformRight * reProjectRight;
}

D3DXMATRIX* ViewAdjustmentMatricies::LeftAdjustmentMatrix()
{
	return &matViewProjTranslateLeft;
}

D3DXMATRIX* ViewAdjustmentMatricies::RightAdjustmentMatrix()
{
	return &matViewProjTranslateRight;
}

D3DXMATRIX* ViewAdjustmentMatricies::Projection()
{
	return &matProjection;
}

D3DXMATRIX* ViewAdjustmentMatricies::ProjectionInverse()
{
	return &matProjectionInv;
}