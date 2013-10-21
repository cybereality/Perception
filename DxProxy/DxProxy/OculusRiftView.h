/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <OculusRiftView.h> and
Class <OculusRiftView> :
Copyright (C) 2012 Andres Hernandez

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

#ifndef OCULUSRIFTVIEW_H_INCLUDED
#define OCULUSRIFTVIEW_H_INCLUDED

#include "StereoView.h"
#include "HMDisplayInfo.h"

/**
* Oculus rift render class.
* Renders in stereo, warped for Oculus Rift.
*/
class OculusRiftView : public StereoView
{
public:
	OculusRiftView(ProxyHelper::ProxyConfig& config, HMDisplayInfo hmd);
	
	/*** OculusRiftView public methods ***/
	virtual void SetViewEffectInitialValues();
	virtual void CalculateShaderVariables( );
	virtual void InitShaderEffects();

private:
	/**
	* Lens center position, Oculus Rift vertex shader constant.
	***/
	float LensCenter[2];
	/**
	* Scales image, Oculus Rift vertex shader constant.
	***/
	float Scale[2];
	/**
	* Maps texture coordinates, Oculus Rift vertex shader constant.
	* ScaleIn maps texture coordinates to Scales to ([-1, 1]), although top/bottom will be larger 
	* due to aspect ratio.
	***/
	float ScaleIn[2];
	/**
	* Predefined Oculus Rift Head Mounted Display info.
	* Contains distortionCoefficients, needed as vertex shader constants
	***/
	HMDisplayInfo hmdInfo;
};

#endif