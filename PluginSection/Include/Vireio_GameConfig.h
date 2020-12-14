/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Vireio_GameConfig.h>
and Class <Vireio_GameConfig>:
Copyright (C) 2015 Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 onwards 2014 by Grant Bagwell, Simon Brown and Neil Schneider
v4.0.x 2015 by Denis Reischl, Grant Bagwell, Simon Brown and Neil Schneider

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

/**
* Vireio Game Configuration since v4.x of the Driver.
* This config replaces the ProxyConfig structure of former versions.
***/
struct Vireio_GameConfiguration
{
	float       fWorldScaleFactor;           /**< Value the eye seperation is to be multiplied with. (mm * worldScaleFactor = mm in game units). */
	float       fConvergence;                /**< Convergence or Neutral Point distance, in meters. **/
	float       fIPD;                        /**< IPD, which stands for interpupillary distance (distance between your pupils - in meters...default = 0.064). Also called the interocular distance (or just Interocular). */
	float       fAspectMultiplier;           /**< Aspect multiplier allows adjusting virtual screen aspect ratio. */
	UINT        dwVRboostMinShaderCount;     /**< Minimum Vertex Shader Count to apply VRboost (security) */
	UINT        dwVRboostMaxShaderCount;     /**< Maximum Vertex Shader Count to apply VRboost (security) */
	bool		bIs64bit;                    /**< The game cpu-architecture, true for 64-bit games */
	int         nRollImpl;                   /**< 0 - NONE, 1 - Matrix Roll, 2 - Pixel Shader Roll */
	bool        bConvergenceEnabled;         /**< Whether convergence is enabled. Typically on for 3D monitors, off for head-mounted displays. **/
	float       fYawMultiplier;              /**< Game-specific tracking multiplier (yaw). */
	float       fPitchMultiplier;            /**< Game-specific tracking multiplier (pitch). */
	float       fRollMultiplier;             /**< Game-specific tracking multiplier (roll). */
	float		fPositionMultiplier;         /**< Game-specific position overall multiplier (for X, Y and Z). */
	float		fPositionXMultiplier;        /**< Game-specific position multiplier for X*/
	float		fPositionYMultiplier;        /**< Game-specific position multiplier for Y*/
	float		fPositionZMultiplier;        /**< Game-specific position multiplier for Z*/
	float		fPFOV;                       /**< Projection FOV, alternative to modifying game's FOV **/
	bool		bPFOVToggle;                 /**< Projection FOV, toggle for above **/
};