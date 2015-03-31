/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Cinema - Virtual Reality Cinema Node Plugin 
Copyright (C) 2014 Denis Reischl

File <VireioCinema.h> and
Class <VireioCinema> :
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

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#define PNT_FLOAT_PLUG_TYPE                          104
#define PNT_INT_PLUG_TYPE                            107 
#define PNT_D3DMATRIX_PLUG_TYPE                     2017
#define PNT_VECTOR3F_PLUG_TYPE                      2061
#define PNT_IDIRECT3DTEXTURE9_PLUG_TYPE             2048

#define NUMBER_OF_COMMANDERS                           2
#define NUMBER_OF_DECOMMANDERS                        13

/**
* Node Commander Enumeration.
***/
enum VRC_Commanders
{
	StereoTextureLeft,
	StereoTextureRight
};

/**
* Node Decommander Enumeration.
***/
enum VRC_Decommanders
{
	Yaw,
	Pitch,
	Roll,
	PositionX,
	PositionY,
	PositionZ,
	FrameTexture,
	ViewOffsetLeft,
	ViewOffsetRight,
	ResolutionWidth,
	ResolutionHeight,
	ProjectionLeft,
	ProjectionRight
};

/**
* Virtual Reality Cinema Node Plugin (Direct3D 9).
***/
class VireioCinema : public AQU_Nodus
{
public:
	VireioCinema();
	virtual ~VireioCinema();

	/*** VireioCinema public methods ***/
	virtual const char*     GetNodeType();
	virtual UINT            GetNodeTypeId();
	virtual LPWSTR          GetCategory();
	virtual HBITMAP         GetLogo();
	virtual DWORD           GetNodeWidth() { return 4+256+4; }
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
	/*** VireioCinema private methods ***/
	void Init(LPDIRECT3DDEVICE9 pcDevice);
	void Render(LPDIRECT3DDEVICE9 pcDevice);
	void SetAllRenderStatesDefault(LPDIRECT3DDEVICE9 pcDevice);

	/**
	* Cinema screen vertex shader.
	***/
	LPDIRECT3DVERTEXSHADER9	m_vsCinema;
	/**
	* Cinema screen pixel shader.
	***/
	LPDIRECT3DPIXELSHADER9	m_psCinema;
	/**
	* Cinema screen pixel shader constant table.
	***/
	LPD3DXCONSTANTTABLE	m_ctPCinema;
	/**
	* Cinema screen pixel shader constant table.
	***/
	LPD3DXCONSTANTTABLE	m_ctVCinema;
	/**
	* Cinema theatre vertex shader.
	***/
	LPDIRECT3DVERTEXSHADER9	m_vsCinemaTheatre;
	/**
	* Cinema theatre pixel shader.
	***/
	LPDIRECT3DPIXELSHADER9	m_psCinemaTheatre;
	/**
	* Cinema theatre pixel shader constant table.
	***/
	LPD3DXCONSTANTTABLE	m_ctPCinemaTheatre;
	/**
	* Cinema theatre pixel shader constant table.
	***/
	LPD3DXCONSTANTTABLE	m_ctVCinemaTheatre;
	/**
	* Cinema screen mesh vertex buffer.
	***/
	LPDIRECT3DVERTEXBUFFER9 m_pVBCinemaScreen;
	/**
	* Cinema theatre mesh vertex buffer.
	***/
	LPDIRECT3DVERTEXBUFFER9 m_pVBCinemaTheatre;
	/**
	* Cinema theatre mesh index buffer.
	***/
	LPDIRECT3DINDEXBUFFER9 m_pIBCinemaTheatre;
	/**
	* Cinema texture.
	***/
	LPDIRECT3DTEXTURE9 m_pTextureCinema;
	/**
	* Cinema theatre texture.
	***/
	LPDIRECT3DTEXTURE9 m_pTextureCinemaTheatre;
	/**
	* Yaw angle pointer.
	***/
	float** m_ppfYaw;
	/**
	* Pitch angle pointer.
	***/
	float** m_ppfPitch;
	/**
	* Roll angle pointer.
	***/
	float** m_ppfRoll;
	/**
	* Yaw angle.
	***/
	float m_fYaw;
	/**
	* Pitch angle.
	***/
	float m_fPitch;
	/**
	* Roll angle.
	***/
	float m_fRoll;
	/**
	* Position X pointer.
	***/
	float** m_ppfPositionX;
	/**
	* Position Y pointer.
	***/
	float** m_ppfPositionY;
	/**
	* Position X pointer.
	***/
	float** m_ppfPositionZ;
	/**
	* Projection Left pointer.
	***/
	D3DMATRIX** m_ppsProjectionLeft;
	/**
	* Projection Left pointer.
	***/
	D3DMATRIX** m_ppsProjectionRight;
	/**
	* View offset pointer left.
	***/
	D3DVECTOR** m_ppsViewOffsetLeft;
	/**
	* View offset pointer right.
	***/
	D3DVECTOR** m_ppsViewOffsetRight;
	/**
	* View offset left.
	***/
	D3DVECTOR m_sViewOffsetLeft;
	/**
	* View offset right.
	***/
	D3DVECTOR m_sViewOffsetRight;
	/**
	* Position vector.
	***/
	D3DVECTOR m_sPositionVector;
	/**
	* The texture of the current frame.
	***/
	LPDIRECT3DTEXTURE9* m_ppcFrameTexture;
	/**
	* The output texture (left).
	***/
	LPDIRECT3DTEXTURE9 m_pStereoOutputLeft;
	/**
	* The output texture (right).
	***/
	LPDIRECT3DTEXTURE9 m_pStereoOutputRight;
	/**
	* The output surface (left).
	***/
	LPDIRECT3DSURFACE9 m_pStereoOutputSurfaceLeft;
	/**
	* The output surface (right).
	***/
	LPDIRECT3DSURFACE9 m_pStereoOutputSurfaceRight;
	/**
	* The depth stencil surface (left).
	***/
	LPDIRECT3DSURFACE9 m_pcDepthStencilLeft;
	/**
	* The depth stencil surface (right).
	***/
	LPDIRECT3DSURFACE9 m_pcDepthStencilRight;
	/**
	* Texture resolution width.
	* Each stereo output texture will have this width.
	***/
	int** m_ppnTexResolutionWidth;
	/**
	* Texture resolution Height.
	* Each stereo output texture will have this height.
	***/
	int** m_ppnTexResolutionHeight;
	/**
	* Render FOV aspect ratio.
	***/
	float m_fAspectRatio;
	/**
	* True if Oculus Tiny Room mesh created.
	* ( to be deleted... )
	***/
	bool m_bOculusTinyRoomMesh;
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	VireioCinema* pVireioCinema = new VireioCinema();
	return static_cast<AQU_Nodus*>(pVireioCinema);
}
