/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

OSVR Tracker - Open Source Virtual Reality Tracking Node
Copyright (C) 2016 Denis Reischl

File <OSVR-Tracker.cpp> and
Class <OSVR-Tracker> :
Copyright (C) 2015 Denis Reischl

The stub class <AQU_Nodus> is the only public class from the Aquilinus 
repository and permitted to be used for open source plugins of any kind. 
Read the Aquilinus documentation for further information.

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

#include"OSVR-Tracker.h"

#define INTERFACE_IDIRECT3DDEVICE9           8

/**
* Constructor.
***/
OSVR_Tracker::OSVR_Tracker():AQU_Nodus()
{
}

/**
* Destructor.
***/
OSVR_Tracker::~OSVR_Tracker()
{
}

/**
* Return the name of the  OSVR Tracker node.
***/
const char* OSVR_Tracker::GetNodeType() 
{
	return "OSVR Tracker"; 
}

/**
* Returns a global unique identifier for the OSVR Tracker node.
***/
UINT OSVR_Tracker::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#define MY_PLUGIN_IDENTIFIER 288
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/**
* Returns the name of the category for the OSVR Tracker node.
***/
LPWSTR OSVR_Tracker::GetCategory()
{
	return L"Motion Tracker";
}

/**
* Returns a logo to be used for the OSVR Tracker node.
***/
HBITMAP OSVR_Tracker::GetLogo() 
{ 
	HMODULE hModule = GetModuleHandle(L"OSVR-Tracker.dll");
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/** 
* Returns the updated control for the OSVR Tracker node.
* Allways return >nullptr< if there is no update for the control !!
***/
HBITMAP OSVR_Tracker::GetControl()
{
	return nullptr;
}

/**
* Provides the name of the requested decommander.
***/
LPWSTR OSVR_Tracker::GetDecommanderName(DWORD dwDecommanderIndex)
{
	//switch ((STS_Decommanders)dwDecommanderIndex)
	{
	//case STS_Decommanders:::
		//return L"";
	}

	return L"";
}

/**
* Returns the plug type for the requested decommander.
***/
DWORD OSVR_Tracker::GetDecommanderType(DWORD dwDecommanderIndex) 
{
	//switch ((STS_Decommanders)dwDecommanderIndex)
	{
	//case STS_Decommanders:::
		//return PNT_IDIRECT3DTEXTURE9_PLUG_TYPE;
	}

	return 0;
}

/**
* Sets the input pointer for the requested decommander.
***/
void OSVR_Tracker::SetInputPointer(DWORD dwDecommanderIndex, void* pData)
{
}

/**
* Tracker supports any calls.
***/
bool OSVR_Tracker::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)  
{ 
	return true;
}

/**
* Handle OSVR tracking.
***/
void* OSVR_Tracker::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)	
{
	return nullptr;
}