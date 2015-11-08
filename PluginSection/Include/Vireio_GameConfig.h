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
	float       worldScaleFactor;           /**< Value the eye seperation is to be multiplied with. (mm * worldScaleFactor = mm in game units). */
	float       convergence;                /**< Convergence or Neutral Point distance, in meters. **/
	float       ipd;                        /**< IPD, which stands for interpupillary distance (distance between your pupils - in meters...default = 0.064). Also called the interocular distance (or just Interocular). */
	float       aspect_multiplier;          /**< Aspect multiplier allows adjusting virtual screen aspect ratio. */
	UINT        VRboostMinShaderCount;      /**< Minimum Vertex Shader Count to apply VRboost (security) */
	UINT        VRboostMaxShaderCount;      /**< Maximum Vertex Shader Count to apply VRboost (security) */
	bool		is64bit;					/**< The game cpu-architecture, true for 64-bit games */
	int         rollImpl;                   /**< 0 - NONE, 1 - Matrix Roll, 2 - Pixel Shader Roll */
	bool        convergenceEnabled;         /**< Whether convergence is enabled. Typically on for 3D monitors, off for head-mounted displays. **/
	float       yaw_multiplier;             /**< Game-specific tracking multiplier (yaw). */
	float       pitch_multiplier;           /**< Game-specific tracking multiplier (pitch). */
	float       roll_multiplier;            /**< Game-specific tracking multiplier (roll). */
	float		position_multiplier;		/**< Game-specific position overall multiplier (for X, Y and Z). */
	float		position_x_multiplier;		/**< Game-specific position multiplier for X*/
	float		position_y_multiplier;		/**< Game-specific position multiplier for Y*/
	float		position_z_multiplier;		/**< Game-specific position multiplier for Z*/
	float		DistortionScale;			/**< The scale to apply distortion. **/
	float		PFOV;						/**< Projection FOV, alternative to modifying game's FOV **/
	bool		PFOVToggle;					/**< Projection FOV, toggle for above **/
};