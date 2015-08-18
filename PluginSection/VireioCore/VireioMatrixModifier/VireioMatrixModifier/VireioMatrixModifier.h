/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Matrix Modifier - Vireio Stereo Matrix Modification Node
Copyright (C) 2015 Denis Reischl

File <VireioMatrixModifier.h> and
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

#include<stdio.h>
#include<vector>

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

#define BYTE_PLUG_TYPE                                 1
#define	FLOAT_PLUG_TYPE                                4
#define INT_PLUG_TYPE                                  7 
#define UINT_PLUG_TYPE                                12
#define PNT_FLOAT_PLUG_TYPE                          104
#define PNT_INT_PLUG_TYPE                            107 
#define PNT_UINT_PLUG_TYPE                           112
#define PNT_VOID_PLUG_TYPE                           114
#define	D3DCOLOR_PLUG_TYPE                          1004
#define D3DFORMAT_PLUG_TYPE                         1011
#define D3DPRIMITIVETYPE_PLUG_TYPE                  1021
#define VECTOR4F_PLUG_TYPE                          1063
#define PNT_D3DRECT_PLUG_TYPE                       2024
#define PNT_ID3D10DEPTHSTENCILVIEW                  8021
#define PNT_ID3D10RENDERTARGETVIEW_TYPE             8022
#define PPNT_ID3D10DEPTHSTENCILVIEW                 9021
#define PPNT_ID3D10RENDERTARGETVIEW                 9022
#define PNT_ID3D11DEPTHSTENCILVIEW                 11025
#define PNT_ID3D11RENDERTARGETVIEW_TYPE            11026
#define PPNT_ID3D11DEPTHSTENCILVIEW                12025
#define PPNT_ID3D11RENDERTARGETVIEW                12026

#define NUMBER_OF_COMMANDERS                           1
#define NUMBER_OF_DECOMMANDERS                         1

/**
* Node Commander Enumeration.
***/
enum STS_Commanders
{
};

/**
* Node Commander Enumeration.
***/
enum STS_Decommanders
{
};

/**
* Vireio Matrix Modifier (DX9/10/11).
* Vireio Perception Stereo Matrix Modification Handler.
***/
class MatrixModifier : public AQU_Nodus
{
public:
	MatrixModifier();
	virtual ~MatrixModifier();

	/*** AQU_Nodus public methods ***/
	virtual const char*     GetNodeType();
	virtual UINT            GetNodeTypeId();
	virtual LPWSTR          GetCategory();
	virtual HBITMAP         GetLogo();
	virtual HBITMAP         GetControl();
	virtual DWORD           GetNodeWidth() { return 4 + 256 + 4; }
	virtual DWORD           GetNodeHeight() { return 128; }
	virtual DWORD           GetCommandersNumber() { return NUMBER_OF_COMMANDERS; }
	virtual DWORD           GetDecommandersNumber() { return NUMBER_OF_DECOMMANDERS; }
	virtual LPWSTR          GetCommanderName(DWORD dwCommanderIndex);
	virtual LPWSTR          GetDecommanderName(DWORD dwDecommanderIndex);
	virtual DWORD           GetCommanderType(DWORD dwCommanderIndex);
	virtual DWORD           GetDecommanderType(DWORD dwDecommanderIndex);
	virtual void*           GetOutputPointer(DWORD dwCommanderIndex);
	virtual void            SetInputPointer(DWORD dwDecommanderIndex, void* pData);
	virtual bool            SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod);
	virtual void*           Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex);

private:
	/*** MatrixModifier private methods ***/
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	MatrixModifier* pMatrixModifier = new MatrixModifier();
	return static_cast<AQU_Nodus*>(pMatrixModifier);
}

