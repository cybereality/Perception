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

#ifndef VIEWADJUSTMENT_H_INCLUDED
#define VIEWADJUSTMENT_H_INCLUDED

#include "d3d9.h"
#include "d3dx9.h"
#include <string>

#define LEFT_CONSTANT -1
#define RIGHT_CONSTANT 1

class ViewAdjustment
{
public:	
	ViewAdjustment();
	virtual ~ViewAdjustment();

	void UpdateProjectionMatrices(float separation, float convergence, float aspectRatio);
	void UpdateRoll(float roll);
	void ComputeViewTranslations(float separation, float convergence, bool rollEnabled);
	
	D3DXMATRIX LeftAdjustmentMatrix();
	D3DXMATRIX RightAdjustmentMatrix();
	D3DXMATRIX Projection();
	D3DXMATRIX ProjectionInverse();

private:

	
	// Projection Matrix variables
	float n;	//Minimum z-value of the view volume
	float f;	//Maximum z-value of the view volume
	float l;	//Minimum x-value of the view volume
	float r;	//Maximum x-value of the view volume
	float t;	//Minimum y-value of the view volume
	float b;	//Maximum y-value of the view volume

	D3DXMATRIX matProjection;
	D3DXMATRIX matProjectionInv;

	D3DXMATRIX reProjectLeft;
	D3DXMATRIX reProjectRight;

	D3DXMATRIX rollMatrix;

	D3DXMATRIX matViewProjTranslateLeft;
	D3DXMATRIX matViewProjTranslateRight;
};



#endif