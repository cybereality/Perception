/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Matrix Modifier - Vireio Stereo Matrix Modification Node
Copyright (C) 2015 Denis Reischl

File <VireioMatrixModifier.cpp> and
Class <VireioMatrixModifier> :
Copyright (C) 2015 Denis Reischl

Parts of this class directly derive from Vireio source code originally
authored by Chris Drain (v1.1.x 2013).

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

#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%u", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"%x", a); OutputDebugString(buf); }

#include"VireioMatrixModifier.h"

#define INTERFACE_ID3D10DEVICE                                               7
#define INTERFACE_ID3D11DEVICECONTEXT                                        11
#define INTERFACE_IDXGISWAPCHAIN                                             29

#define METHOD_IDXGISWAPCHAIN_PRESENT                                        8

#define SAFE_RELEASE(a) if (a) { a->Release(); a = nullptr; }

/**
* Constructor.
***/
MatrixModifier::MatrixModifier() : AQU_Nodus()
{
}

/**
* Destructor.
***/
MatrixModifier::~MatrixModifier()
{
}

/**
* Return the name of the Matrix Modifier node.
***/
const char* MatrixModifier::GetNodeType()
{
	return "Matrix Modifier";
}

/**
* Returns a global unique identifier for the Matrix Modifier node.
***/
UINT MatrixModifier::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#define MY_PLUGIN_IDENTIFIER 75
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/**
* Returns the name of the category for the Matrix Modifier node.
***/
LPWSTR MatrixModifier::GetCategory()
{
	return L"Vireio Core";
}

/**
* Returns a logo to be used for the Matrix Modifier node.
***/
HBITMAP MatrixModifier::GetLogo()
{
	HMODULE hModule = GetModuleHandle(L"VireioMatrixModifier.dll");
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/**
* Returns the updated control for the Matrix Modifier node.
* Allways return >nullptr< if there is no update for the control !!
***/
HBITMAP MatrixModifier::GetControl()
{
	return nullptr;
}

/**
* Provides the name of the requested commander.
***/
LPWSTR MatrixModifier::GetCommanderName(DWORD dwCommanderIndex)
{
	/*switch ((STS_Commanders)dwCommanderIndex)
	{
	case STS_Commanders::XXX:
		return L"XXX";
	}*/

	return L"A";
}

/**
* Provides the name of the requested decommander.
***/
LPWSTR MatrixModifier::GetDecommanderName(DWORD dwDecommanderIndex)
{

	return L"A";
}

/**
* Returns the plug type for the requested commander.
***/
DWORD MatrixModifier::GetCommanderType(DWORD dwCommanderIndex)
{
	return NULL;
}

/**
* Returns the plug type for the requested decommander.
***/
DWORD MatrixModifier::GetDecommanderType(DWORD dwDecommanderIndex)
{
	return 0;
}

/**
* Provides the output pointer for the requested commander.
***/
void* MatrixModifier::GetOutputPointer(DWORD dwCommanderIndex)
{
	//switch((STS_Commanders)dwCommanderIndex)
	//{
	///*case STS_Commanders::StereoTextureLeft:
	//	return (void*)&m_pcStereoOutputLeft;
	//case STS_Commanders::StereoTextureRight:
	//	return (void*)&m_pcStereoOutputRight;*/
	//}

	return nullptr;
}

/**
* Sets the input pointer for the requested decommander.
***/
void MatrixModifier::SetInputPointer(DWORD dwDecommanderIndex, void* pData)
{

}

/**
* Matrix Modifier supports various D3D10 + D3D11 calls.
***/
bool MatrixModifier::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
	if ((nD3DVersion >= (int)AQU_DirectXVersion::DirectX_10) &&
		(nD3DVersion <= (int)AQU_DirectXVersion::DirectX_11_2))
	{
		if (nD3DInterface == INTERFACE_ID3D10DEVICE)
		{
			
		}
		else if (nD3DInterface == INTERFACE_ID3D11DEVICECONTEXT)
		{
			
		}
		else if (nD3DInterface == INTERFACE_IDXGISWAPCHAIN)
		{
			
		}
	}
	return false;
}

/**
* Handle Stereo Render Targets (+Depth Buffers).
***/
void* MatrixModifier::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)
{
	return nullptr;
}