/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Stereo Presenter - Vireio Perception Stereo Configuration Handler
Copyright (C) 2015 Denis Reischl

File <VireioStereoPresenter.h> and
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

#include"AQU_Nodus.h"
#include"Resources.h"

#include <DXGI.h>
#pragma comment(lib, "DXGI.lib")

#include <d3d11_1.h>
#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <d3d10_1.h>
#pragma comment(lib, "d3d10_1.lib")

#include <d3d10.h>
#pragma comment(lib, "d3d10.lib")

#include <d3dx10.h>
#pragma comment(lib, "d3dx10.lib")

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include"..\..\..\Include\Vireio_GUIDs.h"
#include"..\..\..\Include\Vireio_DX11Basics.h"
#include"..\..\..\Include\Vireio_Node_Plugtypes.h"

#define NUMBER_OF_DECOMMANDERS                         7

/**
* Node Commander Enumeration.
***/
enum STP_Decommanders
{
	LeftTexture11,
	RightTexture11,
	LeftTexture10,
	RightTexture10,
	LeftTexture9,
	RightTexture9,
	ViewAdjustments
};

/**
* Available stereo output modes (only monitor modes here).
***/
enum VireioMonitorStereoModes
{
	Vireio_Mono = 0,
	Vireio_SideBySide = 1,
};

/**
* Vireio Stereo Presenter Node Plugin (Direct3D 9).
* Vireio Perception Stereo Drawing Handler.
***/
class StereoPresenter : public AQU_Nodus
{
public:
	StereoPresenter();
	virtual ~StereoPresenter();

	/*** AQU_Nodus public methods ***/
	virtual const char*     GetNodeType();
	virtual UINT            GetNodeTypeId();
	virtual LPWSTR          GetCategory();
	virtual HBITMAP         GetLogo();
	virtual HBITMAP         GetControl();
	virtual DWORD           GetNodeWidth() { return 4 + 256 + 4; }
	virtual DWORD           GetNodeHeight() { return 128; }
	virtual DWORD           GetDecommandersNumber() { return NUMBER_OF_DECOMMANDERS; }
	virtual LPWSTR          GetDecommanderName(DWORD dwDecommanderIndex);
	virtual DWORD           GetDecommanderType(DWORD dwDecommanderIndex);
	virtual void            SetInputPointer(DWORD dwDecommanderIndex, void* pData);
	virtual bool            SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod);
	virtual void*           Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex);

private:
	/**
	* Stereo Textures input. (DX11)
	***/
	ID3D11ShaderResourceView** m_ppcTexView11[2];
	/**
	* True if a stereo mode is selected.
	***/
	VireioMonitorStereoModes m_eStereoMode;
	/**
	* Hotkey switch.
	***/
	bool m_bHotkeySwitch;
	/**
	* The back buffer render target view (DX11).
	***/
	ID3D11RenderTargetView* m_pcBackBufferView;
	/**
	* The 2D vertex shader.
	***/
	union
	{
		ID3D10VertexShader* m_pcVertexShader10;
		ID3D11VertexShader* m_pcVertexShader11;
	};
	/**
	* The 2D pixel shader.
	***/
	union
	{
		ID3D10PixelShader* m_pcPixelShader10;
		ID3D11PixelShader* m_pcPixelShader11;
	};
	/**
	* The 2D vertex layout.
	***/
	union
	{
		ID3D10InputLayout* m_pcVertexLayout10;
		ID3D11InputLayout* m_pcVertexLayout11;
	};
	/**
	* The 2D vertex buffer.
	***/
	union
	{
		ID3D10Buffer* m_pcVertexBuffer10;
		ID3D11Buffer* m_pcVertexBuffer11;
	};
	/**
	* The constant buffer for the vertex shader matrix.
	* Contains only ProjView matrix.
	***/
	union
	{
		ID3D10Buffer* m_pcConstantBufferDirect10;
		ID3D11Buffer* m_pcConstantBufferDirect11;
	};
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	StereoPresenter* pStereoPresenter = new StereoPresenter();
	return static_cast<AQU_Nodus*>(pStereoPresenter);
}
