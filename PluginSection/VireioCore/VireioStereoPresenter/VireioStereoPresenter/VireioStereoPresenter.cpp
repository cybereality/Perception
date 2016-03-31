/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Stereo Presenter - Vireio Perception Stereo Configuration Handler
Copyright (C) 2015 Denis Reischl

File <VireioStereoPresenter.cpp> and
Class <VireioStereoPresenter> :
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

#include"VireioStereoPresenter.h"

#define INTERFACE_ID3D11DEVICE                                               6
#define INTERFACE_ID3D10DEVICE                                               7
#define INTERFACE_IDIRECT3DDEVICE9                                           8
#define INTERFACE_IDXGISWAPCHAIN                                             29

#define METHOD_IDXGISWAPCHAIN_PRESENT                                        8

/**
* Constructor.
***/
StereoPresenter::StereoPresenter() :AQU_Nodus()
{
	m_ppcTexView11[0] = nullptr;
	m_ppcTexView11[1] = nullptr;
}

/**
* Destructor.
***/
StereoPresenter::~StereoPresenter()
{
}

/**
* Return the name of the  Stereo Presenter node.
***/
const char* StereoPresenter::GetNodeType()
{
	return "Stereo Presenter";
}

/**
* Returns a global unique identifier for the Stereo Presenter node.
***/
UINT StereoPresenter::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#define MY_PLUGIN_IDENTIFIER 70
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/**
* Returns the name of the category for the Stereo Presenter node.
***/
LPWSTR StereoPresenter::GetCategory()
{
	return L"Vireio Core";
}

/**
* Returns a logo to be used for the Stereo Presenter node.
***/
HBITMAP StereoPresenter::GetLogo()
{
	HMODULE hModule = GetModuleHandle(L"VireioStereoPresenter.dll");
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/**
* Returns the updated control for the Stereo Presenter node.
* Allways return >nullptr< if there is no update for the control !!
***/
HBITMAP StereoPresenter::GetControl()
{
	return nullptr;
}

/**
* Provides the name of the requested decommander.
***/
LPWSTR StereoPresenter::GetDecommanderName(DWORD dwDecommanderIndex)
{
	switch ((STP_Decommanders)dwDecommanderIndex)
	{
		case STP_Decommanders::LeftTexture11:
			return L"Left Texture DX11";
		case STP_Decommanders::RightTexture11:
			return L"Right Texture DX11";
	}

	return L"";
}

/**
* Returns the plug type for the requested decommander.
***/
DWORD StereoPresenter::GetDecommanderType(DWORD dwDecommanderIndex)
{
	switch ((STP_Decommanders)dwDecommanderIndex)
	{
		case STP_Decommanders::LeftTexture11:
			return NOD_Plugtype::AQU_PNT_ID3D11SHADERRESOURCEVIEW;
		case STP_Decommanders::RightTexture11:
			return NOD_Plugtype::AQU_PNT_ID3D11SHADERRESOURCEVIEW;
	}

	return 0;
}

/**
* Sets the input pointer for the requested decommander.
***/
void StereoPresenter::SetInputPointer(DWORD dwDecommanderIndex, void* pData)
{
	switch ((STP_Decommanders)dwDecommanderIndex)
	{
		case STP_Decommanders::LeftTexture11:
			m_ppcTexView11[0] = (ID3D11ShaderResourceView**)pData;
			break;
		case STP_Decommanders::RightTexture11:
			m_ppcTexView11[1] = (ID3D11ShaderResourceView**)pData;
			break;
	}
}

/**
* Stereo Presenter supports D3D9 to D3D11 Present() and EndScene() calls.
***/
bool StereoPresenter::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
	if ((nD3DInterface == INTERFACE_IDXGISWAPCHAIN) && (nD3DMethod == METHOD_IDXGISWAPCHAIN_PRESENT))
		return true;

	return false;
}

/**
* Handle Stereo Drawing.
***/
void* StereoPresenter::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)
{
	if ((eD3DInterface == INTERFACE_IDXGISWAPCHAIN) && (eD3DMethod == METHOD_IDXGISWAPCHAIN_PRESENT))
	{

	}
	return nullptr;
}