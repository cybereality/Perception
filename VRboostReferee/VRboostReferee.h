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
#include "VRBoostEnums.h"

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
