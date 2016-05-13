/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

OpenVR DirectMode - Open Virtual Reality Direct Mode Rendering Node
Copyright (C) 2016 Denis Reischl

File <OpenVR-DirectMode.cpp> and
Class <OpenVR-DirectMode> :
Copyright (C) 2016 Denis Reischl

The stub class <AQU_Nodus> is the only public class from the Aquilinus
repository and permitted to be used for open source plugins of any kind.
Read the Aquilinus documentation for further information.

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 to v3.0.x 2014-2015 by Grant Bagwell, Simon Brown and Neil Schneider
v4.0.x 2015 by Denis Reischl, Grant Bagwell, Simon Brown, Samuel Austin
and Neil Schneider

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

#include"OpenVR-DirectMode.h"

/**
* Constructor.
***/
OpenVR_DirectMode::OpenVR_DirectMode() : AQU_Nodus()
{
}

/**
* Destructor.
***/
OpenVR_DirectMode::~OpenVR_DirectMode()
{

}

/**
* Return the name of the  OpenVR DirectMode node.
***/
const char* OpenVR_DirectMode::GetNodeType()
{
	return "OpenVR DirectMode";
}

/**
* Returns a global unique identifier for the OpenVR DirectMode node.
***/
UINT OpenVR_DirectMode::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#define MY_PLUGIN_IDENTIFIER 321
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/**
* Returns the name of the category for the OpenVR DirectMode node.
***/
LPWSTR OpenVR_DirectMode::GetCategory()
{
	return L"Renderer";
}

/**
* Returns a logo to be used for the OpenVR DirectMode node.
***/
HBITMAP OpenVR_DirectMode::GetLogo()
{
	HMODULE hModule = GetModuleHandle(L"OpenVR-DirectMode.dll");
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/**
* Returns the updated control for the OpenVR DirectMode node.
* Allways return >nullptr< if there is no update for the control !!
***/
HBITMAP OpenVR_DirectMode::GetControl()
{
	return nullptr;
}

/**
* Provides the name of the requested commander.
***/
LPWSTR OpenVR_DirectMode::GetCommanderName(DWORD dwCommanderIndex)
{

	return L"";
}

/**
* Provides the type of the requested commander.
***/
DWORD OpenVR_DirectMode::GetCommanderType(DWORD dwCommanderIndex)
{

	return 0;
}

/**
* Provides the pointer of the requested commander.
***/
void* OpenVR_DirectMode::GetOutputPointer(DWORD dwCommanderIndex)
{

	return nullptr;
}

/**
* DirectMode supports any calls.
***/
bool OpenVR_DirectMode::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
	return true;
}

/**
* Handle OpenVR tracking.
***/
void* OpenVR_DirectMode::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)
{

	return nullptr;
}