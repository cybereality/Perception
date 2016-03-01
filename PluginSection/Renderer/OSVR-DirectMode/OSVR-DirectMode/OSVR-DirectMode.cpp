/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

OSVR Direct Mode - Open Source Virtual Reality Direct Mode Node
Copyright (C) 2016 Denis Reischl

File <OSVR-DirectMode.cpp> and
Class <OSVR-DirectMode> :
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

#include"OSVR-DirectMode.h"

#define INTERFACE_IDIRECT3DDEVICE9           8

/**
* Constructor.
***/
OSVR_DirectMode::OSVR_DirectMode() :AQU_Nodus()
{
}

/**
* Destructor.
***/
OSVR_DirectMode::~OSVR_DirectMode()
{
}

/**
* Return the name of the  OSVR Direct Mode node.
***/
const char* OSVR_DirectMode::GetNodeType()
{
	return "OSVR Direct Mode";
}

/**
* Returns a global unique identifier for the OSVR Direct Mode node.
***/
UINT OSVR_DirectMode::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#define MY_PLUGIN_IDENTIFIER 289
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/**
* Returns the name of the category for the OSVR Direct Mode node.
***/
LPWSTR OSVR_DirectMode::GetCategory()
{
	return L"Renderer";
}

/**
* Returns a logo to be used for the OSVR Direct Mode node.
***/
HBITMAP OSVR_DirectMode::GetLogo()
{
	HMODULE hModule = GetModuleHandle(L"OSVR-DirectMode.dll");
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/**
* Returns the updated control for the OSVR Direct Mode node.
* Allways return >nullptr< if there is no update for the control !!
***/
HBITMAP OSVR_DirectMode::GetControl()
{
	return nullptr;
}

/**
* Provides the name of the requested commander.
***/
LPWSTR OSVR_DirectMode::GetCommanderName(DWORD dwCommanderIndex)
{
	//switch ((OSVR_Commanders)dwCommanderIndex)
	{
	}

	return L"";
}

/**
* Provides the type of the requested commander.
***/
DWORD OSVR_DirectMode::GetCommanderType(DWORD dwCommanderIndex)
{
	//switch ((OSVR_Commanders)dwCommanderIndex)
	{
	}

	return 0;
}

/**
* Provides the pointer of the requested commander.
***/
void* OSVR_DirectMode::GetOutputPointer(DWORD dwCommanderIndex)
{
	//switch ((OSVR_Commanders)dwCommanderIndex)
	{
	}

	return nullptr;
}

/**
* DirectMode supports any calls.
***/
bool OSVR_DirectMode::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
	return true;
}

/**
* Handle OSVR direct mode.
***/
void* OSVR_DirectMode::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)
{
	return nullptr;
}