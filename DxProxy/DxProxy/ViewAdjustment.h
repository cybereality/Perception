/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <ViewAdjustment.h> and
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
	ViewAdjustment(HMDisplayInfo *hmdInfo, ProxyConfig *config);
	virtual ~ViewAdjustment();

	/*** ViewAdjustment public methods ***/
	void          Load(ProxyConfig& cfg);
	void          Save(ProxyConfig& cfg);
	void          UpdateProjectionMatrices(float aspectRatio, float fov_horiz);
	void          UpdateRoll(float roll);
	void		  UpdatePosition(float yaw, float pitch, float roll, float xPosition = 0.0f, float yPosition = 0.0f, float zPosition = 0.0f);
	void          ComputeViewTransforms(); 
	
	/*** Getters for pre-computed matrices ***/
	
	D3DXMATRIX PositionMatrix()              { return matPosition; }
	D3DXMATRIX LeftAdjustmentMatrix()        { return matViewProjTransformLeft; }
	D3DXMATRIX RightAdjustmentMatrix()       { return matViewProjTransformRight; }
	D3DXMATRIX LeftAdjustmentMatrixNoRoll()  { return matViewProjTransformLeftNoRoll; }
	D3DXMATRIX RightAdjustmentMatrixNoRoll() { return matViewProjTransformRightNoRoll; }
	D3DXMATRIX LeftView()                    { return matViewProjLeft; }
	D3DXMATRIX RightView()                   { return matViewProjRight; }
	D3DXMATRIX LeftViewTransform()           { return transformLeft; }
	D3DXMATRIX RightViewTransform()          { return transformRight; }
	D3DXMATRIX Projection()                  { return matBasicProjection; }
	D3DXMATRIX ProjectionInverse()           { return matProjectionInv; }
	D3DXMATRIX RollMatrix()                  { return rollMatrix; }
	D3DXMATRIX RollMatrixNegative()          { return rollMatrixNegative; }
	D3DXMATRIX RollMatrixHalf()              { return rollMatrixHalf; }
	D3DXMATRIX LeftHUDMatrix()               { return matHudLeft; }
	D3DXMATRIX RightHUDMatrix()              { return matHudRight; }
	D3DXMATRIX LeftGUIMatrix()               { return matGuiLeft; }
	D3DXMATRIX RightGUIMatrix()              { return matGuiRight; }
	D3DXMATRIX Squash()                      { return matSquash; }
	D3DXMATRIX HUDDistance()                 { return matHudDistance; }
	D3DXMATRIX LeftHUD3DDepth()              { return matLeftHud3DDepth; }
	D3DXMATRIX RightHUD3DDepth()             { return matRightHud3DDepth; }
	D3DXMATRIX LeftHUD3DDepthShifted()       { return matLeftHud3DDepthShifted; }
	D3DXMATRIX RightHUD3DDepthShifted()      { return matRightHud3DDepthShifted; }
	D3DXMATRIX LeftGUI3DDepth()              { return matLeftGui3DDepth; }
	D3DXMATRIX RightGUI3DDepth()             { return matRightGui3DDepth; }
	D3DXMATRIX GatheredMatrixLeft()          { return matGatheredLeft; }
	D3DXMATRIX GatheredMatrixRight()         { return matGatheredRight; }
	
	void          GatherMatrix(D3DXMATRIX& matrixLeft, D3DXMATRIX& matrixRight);
	float         ChangeWorldScale(float toAdd);
	float         SetConvergence(float newConvergence);
	float         ChangeConvergence(float toAdd);
	void          ChangeGUISquash(float newSquash);
	void          ChangeGUI3DDepth(float newGui3DDepth);
	void          ChangeHUDDistance(float newHudDistance);
	void          ChangeHUD3DDepth(float newHud3DDepth);
	void		  SetGameSpecificPositionalScaling(D3DXVECTOR3 scalingVec);
	float         Convergence();
	float         ConvergenceInWorldUnits();
	float         SeparationInWorldUnits();
	float         SeparationIPDAdjustment();
	//int           RollImpl();
	//void          SetRollImpl(int rollImpl);
	//int			  GetStereoType();
	void		  SetUsePFOV(bool);
	HMDisplayInfo* HMDInfo();	

private:
	ProxyConfig *config;
	
	bool m_usePFOV;
	
	D3DXVECTOR3 positionTransformVec;

	/**
	* Positional translation matrix
	**/
	D3DXMATRIX matPosition;
	/**
	* Projection matrix - basic with no PFOV
	***/
	D3DXMATRIX matBasicProjection;
	/**
	* Projection inverse matrix.
	***/
	D3DXMATRIX matProjectionInv;
	/**
	* The projection with adjusted FOV.
	***/
	D3DXMATRIX projectPFOV;
	/**
	* The projections with left and right eye convergence.
	***/
	D3DXMATRIX projectLeftConverge, projectRightConverge;
	/**
	* The head roll matrix.
	***/
	D3DXMATRIX rollMatrix;
	/**
	* The head roll matrix. (negative)
	***/
	D3DXMATRIX rollMatrixNegative;
	/**
	* The head roll matrix. (half roll)
	***/
	D3DXMATRIX rollMatrixHalf;
	/**
	* Right and left matrices used to roll (if roll enabled) and shift view for ipd.
	***/
	D3DXMATRIX transformLeft, transformRight;
	/**
	* Left and right view projection matrices
	***/
	D3DXMATRIX matViewProjLeft, matViewProjRight;
	/**
	* Left and right view projection transform matrices.
	***/
	D3DXMATRIX matViewProjTransformLeft, matViewProjTransformRight;
	/**
	* Left and right view projection transform matrix.
	***/
	D3DXMATRIX matViewProjTransformLeftNoRoll, matViewProjTransformRightNoRoll;
	/**
	* Gathered matrices to be used in gathered modifications.
	***/
	D3DXMATRIX matGatheredLeft, matGatheredRight;
	/**
	* Left and right HUD matrices.
	***/
	D3DXMATRIX matHudLeft, matHudRight;
	/**
	* Left and right GUI matrices.
	***/
	D3DXMATRIX matGuiLeft, matGuiRight;
	/**
	* Squash scaling matrix, to be used in HUD/GUI scaling matrices.
	***/
	D3DXMATRIX matSquash;
	/**
	* HUD distance matrix, to be used in HUD scaling matrices.
	***/
	D3DXMATRIX matHudDistance;
	/**
	* HUD 3d depth matrices, to be used in HUD separation matrices.
	***/
	D3DXMATRIX matLeftHud3DDepth, matRightHud3DDepth;
	/**
	* HUD 3d depth matrices, to be used in HUD separation matrices.
	***/
	D3DXMATRIX matLeftHud3DDepthShifted, matRightHud3DDepthShifted;
	/**
	* HUD 3d depth matrices, to be used in HUD separation matrices.
	***/
	D3DXMATRIX matLeftGui3DDepth, matRightGui3DDepth;
	/**
	* Used to scale the positional movement, seems x/y/z are not equal
	*/
	D3DXVECTOR3 gameScaleVec;
	/**
	* Head mounted display info.
	***/
	HMDisplayInfo* hmdInfo;
	/**
	* Amount of actual roll (in radians)
	*/
	float m_roll;
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
	/**
	* The amount of squashing GUI shader constants.
	* 1.0 == full render
	***/
	float squash;
	/**
	* The 3d depth of the GUI.
	***/
	float gui3DDepth;
	/**
	* The distance of the HUD.
	***/
	float hudDistance;
	/**
	* The 3d depth of the HUD.
	***/
	float hud3DDepth;
};
#endif