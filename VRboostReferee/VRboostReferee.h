/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <VRboostReferee.h> :
Copyright (C) 2013 Denis Reischl

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
#pragma once

#include <SDKDDKVer.h>

#include <d3d9.h>
#include "d3dx9.h"
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include "..//DxProxy//DxProxy//pugixml.hpp"

/**
* Float modification identifiers. 
***/
enum FloatModificationTypes
{
	FloatDoNothing = 0,               /**< Simple modification that does not apply anything. **/
	FloatSimpleApply = 1,             /**< Simply applies the input float to the destination. **/
	FloatSimpleNegativeApply = 2,     /**< Simply applies the negative input float to the destination. **/
	FloatScale = 3,                   /**< Applies a scaled float. */
	FloatToDoubleScale = 4,           /**< Inputs a float and applies a scaled double. */
	FloatToBoolScale = 5,             /**< Inputs a float and applies a scaled bool. */
	FloatToByteScale = 6,             /**< Inputs a float and applies a scaled byte. */
	FloatToShortScale = 7,            /**< Inputs a float and applies a scaled short. */
	FloatToWordScale = 8,             /**< Inputs a float and applies a scaled WORD. */
	FloatToIntegerScale = 9,          /**< Inputs a float and applies a scaled int. */
	FloatToDWordScale = 10,           /**< Inputs a float and applies a scaled DWORD. */
	FloatToLongScale = 11,            /**< Inputs a float and applies a scaled long. */
	FloatToQWordScale = 12,           /**< Inputs a float and applies a scaled double. */
	FloatDegreeApply = 80,            /**< Applies degrees from radian input. */
	FloatDegreeCompass = 81,          /**< Applies degrees from radian input and incorporates prior rotation angle. **/
	FloatDegreeNegativeApply = 82,    /**< Applies degrees from radian input. */
	FloatDegreeNegativeCompass = 83,  /**< Applies degrees from radian input and incorporates prior rotation angle. **/
	FloatGaussianCompass = 100,       /**< Applies the input float to a gaussian compass calculatian and incorporates prior rotation angle. **/
	FloatUnrealCompass = 101,         /**< Applies the input float to a UNREAL engine rotator compass calculatian and incorporates prior rotation angle. **/
	FloatUnrealAxis = 201,            /**< Applies the input float to a UNREAL engine rotator axis. **/
	FloatUnrealNegativeAxis = 202,    /**< Applies the input float to a negative UNREAL engine rotator axis. **/
};

/**
* VRboost axis table.
* Here, we set indices for the to-be-assigned VRboost input register.
***/
enum VRboostAxis
{
	TrackerYaw = 0,                  /**< This index is reserved for Tracker->primaryYaw. **/
	TrackerPitch = 1,                /**< This index is reserved for Tracker->primaryPitch. **/
	TrackerRoll = 2,                 /**< This index is reserved for Tracker->primaryRoll. **/
	Zero = 3,                        /**< This index is reserved for constant 0 output. **/
	One = 4,                         /**< This index is reserved for constant 1 output. **/
	// 5..11                              reserved for future tracker use
	// 12..23                             reserved for future input device use
	WorldFOV = 24,                   /**< Game-specific global FOV setting. **/
	PlayerFOV = 25,                  /**< Game-specific player surroundings FOV setting. **/
	FarPlaneFOV = 26,                /**< Game-specific FOV setting for the most distant objects. **/ 
	CameraTranslateX = 27,           /**< Translates the camera along the X-Axis. **/
	CameraTranslateY = 28,           /**< Translates the camera along the Y-Axis. **/
	CameraTranslateZ = 29,           /**< Translates the camera along the Z-Axis. **/
	CameraDistance = 30,             /**< Changes the distance of the camera. **/
	CameraZoom = 31,                 /**< Changes the cameras zoom. **/
	CameraHorizonAdjustment = 32,    /**< Adjusts the cameras X axis rotation to match the horizon. **/
	ConstantValue1 = 33,             /**< Game-specific constant value 1. **/
	ConstantValue2 = 34,             /**< Game-specific constant value 2. **/
	ConstantValue3 = 35,             /**< Game-specific constant value 3. **/
	// 255         maximum VRboost axis index
};


/*** VRboost function pointer typedefs ***/
typedef HRESULT (WINAPI *LPVRBOOST_LoadMemoryRules)(std::string processName, std::string rulesPath);
typedef HRESULT (WINAPI *LPVRBOOST_SaveMemoryRules)(std::string rulesPath);
typedef HRESULT (WINAPI *LPVRBOOST_CreateFloatMemoryRule)(DWORD ruleType, UINT axisIndex, D3DXVECTOR4 constantVector, DWORD pointerAddress, DWORD* offsets, DWORD minValue, DWORD maxValue, DWORD comparisationPointer1, DWORD* comparisationOffsets1, int pointerDifference1, DWORD comparisationPointer2, DWORD* comparisationOffsets2, int pointerDifference2);
typedef HRESULT (WINAPI *LPVRBOOST_SetProcess)(std::string processName, std::string moduleName);
typedef HRESULT (WINAPI *LPVRBOOST_ReleaseAllMemoryRules)( void );
typedef HRESULT (WINAPI *LPVRBOOST_ApplyMemoryRules)(UINT axisNumber, float** axis);

/**
* VRboost pointer function to load memory rules for a process.
***/
LPVRBOOST_LoadMemoryRules m_pVRboost_LoadMemoryRules;
/**
* VRboost pointer function to load memory rules for a process.
***/
LPVRBOOST_SaveMemoryRules m_pVRboost_SaveMemoryRules;
/**
* VRboost pointer function to create a float memory rule.
***/
LPVRBOOST_CreateFloatMemoryRule m_pVRboost_CreateFloatMemoryRule;
/**
* VRboost pointer function to load memory rules for a process.
***/
LPVRBOOST_SetProcess m_pVRboost_SetProcess;
/**
* VRboost pointer function to release all memory rules.
***/
LPVRBOOST_ReleaseAllMemoryRules m_pVRboost_ReleaseAllMemoryRules;
/**
* VRboost pointer function to apply memory rules to process memory.
***/
LPVRBOOST_ApplyMemoryRules m_pVRboost_ApplyMemoryRules;
