/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Oculus Renderer - Oculus Rift Renderer Node Plugin
Copyright (C) 2015 Denis Reischl

File <OculusRenderer.h> and
Class <OculusRenderer> :
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
#include"ORN_LibOVR.h"

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#define PNT_FLOAT_PLUG_TYPE                          104
#define PNT_INT_PLUG_TYPE                            107 
#define PNT_UINT_PLUG_TYPE                           112
#define PNT_VECTOR4F_PLUG_TYPE                      2063
#define PNT_IDIRECT3DTEXTURE9_PLUG_TYPE             2048
#define PPNT_IDIRECT3DINDEXBUFFER9_PLUG_TYPE        3041
#define PPNT_IDIRECT3DVERTEXBUFFER9_PLUG_TYPE       3049
#define PPNT_IDIRECT3DVERTEXDECLARATION9_PLUG_TYPE  3050

#define NUMBER_OF_DECOMMANDERS                         9


/**
* Node Commander Enumeration.
***/
enum ORN_Decommanders
{
	LeftTexture,
	RightTexture,
	DistortionVertexBufferLeft,  /**<  Distortion mesh for the left eye. (D3D9) **/
	DistortionVertexBufferRight, /**<  Distortion mesh for the right eye. (D3D9) **/
	DistortionIndexBufferLeft,   /**<  Index buffer for the distortion mesh, left. **/
	DistortionIndexBufferRight,  /**<  Index buffer for the distortion mesh, right. **/
	OculusVertexDeclaration,     /**<  Vertex declaration (D3D9) for the distortion vertex shader. **/
	DefaultEyeFovLeft,           /**<  Recommended optical ﬁeld of view for each eye. (Left) **/
	DefaultEyeFovRight,          /**<  Recommended optical ﬁeld of view for each eye. (Right) **/
};

/**
* Virtual Reality Oculus Node Plugin (Direct3D 9).
***/
class OculusRenderer : public AQU_Nodus
{
public:
	OculusRenderer();
	virtual ~OculusRenderer();

	/*** AQU_Nodus public methods ***/
	virtual const char*     GetNodeType();
	virtual UINT            GetNodeTypeId();
	virtual LPWSTR          GetCategory();
	virtual HBITMAP         GetLogo();
	virtual HBITMAP         GetControl();
	virtual DWORD           GetNodeWidth() { return 4+256+4; }
	virtual DWORD           GetNodeHeight() { return 128; }
	virtual DWORD           GetDecommandersNumber() { return NUMBER_OF_DECOMMANDERS; }
	virtual LPWSTR          GetDecommanderName(DWORD dwDecommanderIndex);
	virtual DWORD           GetDecommanderType(DWORD dwDecommanderIndex);
	virtual void            SetInputPointer(DWORD dwDecommanderIndex, void* pData);
	virtual bool            SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod);
	virtual void*           Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex);

private:
	/*** OculusRenderer private methods ***/
	void                    SetAllRenderStatesDefault(LPDIRECT3DDEVICE9 pDevice);

	/**
	* Pointers to input data. 
	***/
	LPVOID m_paInput[NUMBER_OF_DECOMMANDERS];
	/**
	* The control bitmap.
	***/
	HBITMAP m_hBitmapControl;
	/**
	* The control update bool.
	***/
	bool m_bControlUpdate;
	/**
	* Oculus pixel shader.
	***/
	LPDIRECT3DPIXELSHADER9	m_pcOculusPixelShader;
	/**
	* Oculus pixel shader constant table.
	***/
	LPD3DXCONSTANTTABLE	m_pcOculusPixelShaderCT;
	/**
	* Oculus vertex shader.
	***/
	LPDIRECT3DVERTEXSHADER9	m_pcOculusVertexShader;
	/**
	* Oculus vertex shader constant table.
	***/
	LPD3DXCONSTANTTABLE	m_pcOculusVertexShaderCT;
	/**
	* Left Eye Texture.
	***/
	LPDIRECT3DTEXTURE9 m_pcTextureLeft;
	/**
	* Right Eye Texture.
	***/
	LPDIRECT3DTEXTURE9 m_pcTextureRight;
	/**
	* Left Eye Test Texture.
	***/
	LPDIRECT3DTEXTURE9 m_pcTextureLeftTest;
	/**
	* Right Eye Test Texture.
	***/
	LPDIRECT3DTEXTURE9 m_pcTextureRightTest;
	/**
	* The direct3d 9 distortion mesh vertex buffer for the left eye.
	***/
	LPDIRECT3DVERTEXBUFFER9 m_pcDistortionVertexBufferLeft;
	/**
	* The direct3d 9 distortion mesh vertex buffer for the right eye.
	***/
	LPDIRECT3DVERTEXBUFFER9 m_pcDistortionVertexBufferRight;
	/**
	* Oculus distortion vertex declaration (D3D9).
	***/
	LPDIRECT3DVERTEXDECLARATION9 m_pcVertexDecl;
	/**
	* Index buffer for the distortion mesh, left. 
	***/
	LPDIRECT3DINDEXBUFFER9 m_pcDistortionIndexBufferLeft;
	/**
	* Index buffer for the distortion mesh, right.
	***/
	LPDIRECT3DINDEXBUFFER9 m_pcDistortionIndexBufferRight;
	/**
	* Vertex buffer description, left.
	***/
	D3DVERTEXBUFFER_DESC m_sVertexBufferDescLeft;
	/**
	* Vertex buffer description, right.
	***/
	D3DVERTEXBUFFER_DESC m_sVertexBufferDescRight;
	/**
	* Index buffer description, left.
	***/
	D3DINDEXBUFFER_DESC m_sIndexBufferDescLeft;
	/**
	* Index buffer description, right.
	***/
	D3DINDEXBUFFER_DESC m_sIndexBufferDescRight;
	/**
	* Default FOV port, left.
	***/
	ovrFovPort m_sDefaultFOVPortLeft;
	/**
	* Default FOV port, right.
	***/
	ovrFovPort m_sDefaultFOVPortRight;
	/**
	* Input FOV port, left.
	***/
	ovrFovPort* m_psFOVPortLeft;
	/**
	* Input FOV port, right.
	***/
	ovrFovPort* m_psFOVPortRight;
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	OculusRenderer* pOculusRenderer = new OculusRenderer();
	return static_cast<AQU_Nodus*>(pOculusRenderer);
}
