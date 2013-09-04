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

#include <string>
#include <memory>
#include "d3d9.h"
#include "d3dx9.h"
#include "HMDisplayInfo.h"
#include "Vireio.h"
#include "ProxyHelper.h"

#define LEFT_CONSTANT -1
#define RIGHT_CONSTANT 1

/**
* Class for eye and head roll adjustment matrix calculation.
* Calculates left and right view projection transform matrices.
*
* ALL MATRICES are identity matrices if worldScaleFactor in game configuration not set (==zero). 
* @see ShaderConstantModification
*/
class ViewAdjustment
{
public:	
	ViewAdjustment(HMDisplayInfo &hmdInfo, float metersToWorldUnits, bool enableRoll);
	virtual ~ViewAdjustment();

	/*** ViewAdjustment public methods ***/
	void          Load(ProxyHelper::ProxyConfig& cfg);
	void          Save(ProxyHelper::ProxyConfig& cfg);
	void          UpdateProjectionMatrices(float aspectRatio);
	void          UpdateRoll(float roll);
	void          ComputeViewTransforms(); 
	D3DXMATRIX    LeftAdjustmentMatrix();
	D3DXMATRIX    RightAdjustmentMatrix();	
	D3DXMATRIX    LeftViewTransform();
	D3DXMATRIX    RightViewTransform();	
	D3DXMATRIX    LeftShiftProjection();
	D3DXMATRIX    RightShiftProjection();
	D3DXMATRIX    Projection();
	D3DXMATRIX    ProjectionInverse();	
	float         ChangeWorldScale(float toAdd);
	float         ChangeConvergence(float toAdd);
	void          ResetWorldScale();
	void          ResetConvergence();	
	float         Convergence();
	float         ConvergenceInWorldUnits();
	float         SeparationInWorldUnits();
	bool          RollEnabled();
	HMDisplayInfo HMDInfo();	

private:
	/***  private methods ***/
	void SetIPD(float ipdInMeters);
			
	/*** Projection Matrix variables ***/
	float n;	/**< Minimum z-value of the view volume. */
	float f;	/**< Maximum z-value of the view volume. */
	float l;	/**< Minimum x-value of the view volume. */
	float r;	/**< Maximum x-value of the view volume. */
	float t;	/**< Minimum y-value of the view volume. */
	float b;	/**< Maximum y-value of the view volume. */

	/**
	* Constant minimum convergence.
	***/
	float minConvergence;
	/**
	* Constant maximum convergence.
	***/
	float maxConvergence;
	/**
	* Projection matrix.
	***/
	D3DXMATRIX matProjection;
	/**
	* Projection inverse matrix.
	***/
	D3DXMATRIX matProjectionInv;
	/**
	* The translation applied to projection to shift it left/right to get project(Left/Right).
	***/
	D3DXMATRIX leftShiftProjection;
	/**
	* The translation applied to projection to shift it left/right to get project(Left/Right).
	***/
	D3DXMATRIX rightShiftProjection;
	/**
	* The shifted left projection matrix.
	***/
	D3DXMATRIX projectLeft;
	/**
	* The shifted right projection matrix.
	***/
	D3DXMATRIX projectRight;
	/**
	* The head roll matrix.
	***/
	D3DXMATRIX rollMatrix;
	/**
	* Left matrix used to roll (if roll enabled) and shift view for ipd.
	***/
	D3DXMATRIX transformLeft;
	/**
	* Right matrix used to roll (if roll enabled) and shift view for ipd.
	***/
	D3DXMATRIX transformRight;
	/**
	* Left view projection transform matrix.
	***/
	D3DXMATRIX matViewProjTransformLeft;
	/**
	* Right view projection transform matrix.
	***/
	D3DXMATRIX matViewProjTransformRight;
	/**
	* Head mounted display info.
	***/
	HMDisplayInfo hmdInfo;
	/**
	* True if head roll enabled.
	***/
	bool rollEnabled;
	/**
	* World scale, used to correct eye seperation game-specific.
	***/
	float metersToWorldMultiplier;
	/**
	* Interpupillary distance.
	* As provided from Oculus Configuration Utility (or set in the "user.xml" file).
	***/
	float ipd;
	/**
	* Convergence.
	* Left/Rigth offset adjustment. In millimeters.
	***/
	float convergence;
};
#endif