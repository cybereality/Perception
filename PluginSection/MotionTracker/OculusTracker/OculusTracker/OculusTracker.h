/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Oculus Tracker - Oculus Rift Motion Tracker Node Plugin
Copyright (C) 2015 Denis Reischl

File <OculusTracker.h> and
Class <OculusTracker> :
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

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include"OVR.h"

#define PNT_FLOAT_PLUG_TYPE                          104
#define PNT_INT_PLUG_TYPE                            107 
#define PNT_UINT_PLUG_TYPE                           112
#define PNT_D3DMATRIX_PLUG_TYPE                     2017
#define PNT_VECTOR3F_PLUG_TYPE                      2061
#define PNT_VECTOR4F_PLUG_TYPE                      2063
#define PPNT_IDIRECT3DINDEXBUFFER9_PLUG_TYPE        3041
#define PPNT_IDIRECT3DVERTEXBUFFER9_PLUG_TYPE       3049
#define PPNT_IDIRECT3DVERTEXDECLARATION9_PLUG_TYPE  3050

#define NUMBER_OF_COMMANDERS                          32

/**
* Node Commander Enumeration.
***/
enum OTR_Commanders
{
	Yaw,
	Pitch,
	Roll,
	OrientationX,
	OrientationY,
	OrientationZ,
	OrientationW,
	PositionX,
	PositionY,
	PositionZ,
	CameraFrustumHFovInRadians,  /**<  The horizontal FOV of the position tracking camera frustum. **/
	CameraFrustumVFovInRadians,  /**<  The vertical FOV of the position tracking camera frustum. **/
	CameraFrustumNearZInMeters,  /**<  The distance from the position tracking camera to the near frustum bounds. **/
	CameraFrustumFarZInMeters,   /**<  The distance from the position tracking camera to the far frustum bounds. **/
	HmdCaps,                     /**<  HMD capability bits described by ovrHmdCaps. **/
	TrackingCaps,                /**<  Tracking capability bits describing whether orientation, position tracking, and yaw drift correction are supported. **/
	DistortionCaps,              /**<  Distortion capability bits describing whether timewarp and chromatic aberration correction are supported. **/
	ResolutionW,                 /**<  Device Resolution Width. **/
	ResolutionH,                 /**<  Device Resolution Height. **/
	DistortionVertexBufferLeft,  /**<  Distortion mesh for the left eye. (D3D9) **/
	DistortionVertexBufferRight, /**<  Distortion mesh for the right eye. (D3D9) **/
	DistortionIndexBufferLeft,   /**<  The index buffer for the left eye. (D3D9) **/
	DistortionIndexBufferRight,  /**<  The index buffer for the right eye. (D3D9) **/
	OculusVertexDeclaration,     /**<  Vertex declaration (D3D9) for the distortion vertex shader. **/
	DefaultEyeFovLeft,           /**<  Recommended optical ﬁeld of view for each eye. (Left) **/
	DefaultEyeFovRight,          /**<  Recommended optical ﬁeld of view for each eye. (Right) **/
	MaxEyeFovLeft,               /**<  Maximum optical ﬁeld of view that can be practically rendered for each eye. (Left) **/
	MaxEyeFovRight,              /**<  Maximum optical ﬁeld of view that can be practically rendered for each eye. (Right) **/
	HmdToEyeViewOffsetLeft,      /**<  Translation to be applied to the view matrix. HMD->Eye (Left) **/
	HmdToEyeViewOffsetRight,     /**<  Translation to be applied to the view matrix. HMD->Eye (Right) **/
	DefaultProjectionMatrixLeft, /**<  Default FOV Oculus Rift projection matrix (Left) **/
	DefaultProjectionMatrixRight,/**<  Default FOV Oculus Rift projection matrix (Left) **/
};

/**
* Vertex declaration for the oculus distortion vertex.
***/
static const D3DVERTEXELEMENT9 VertexElements[7] =	{
	{ 0,  0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0,  8, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 1 },
	{ 0, 12, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 2 },
	{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
	{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
	D3DDECL_END()	};

/**
* Oculus Tracker Node Plugin (Direct3D 9).
***/
class OculusTracker : public AQU_Nodus
{
public:
	OculusTracker();
	virtual ~OculusTracker();

	/*** OculusTracker public methods ***/
	virtual const char*     GetNodeType();
	virtual UINT            GetNodeTypeId();
	virtual LPWSTR          GetCategory();
	virtual HBITMAP         GetLogo();
	virtual HBITMAP         GetControl();
	virtual DWORD           GetNodeWidth() { return 4+256+4; }
	virtual DWORD           GetNodeHeight() { return 128; }
	virtual DWORD           GetCommandersNumber() { return NUMBER_OF_COMMANDERS; }
	virtual LPWSTR          GetCommanderName(DWORD dwCommanderIndex);
	virtual DWORD           GetCommanderType(DWORD dwCommanderIndex);
	virtual void*           GetOutputPointer(DWORD dwCommanderIndex);
	virtual bool            SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod);
	virtual void*           Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex);

private:
	/**
	* All output pointers.
	***/
	LPVOID m_paOutput[NUMBER_OF_COMMANDERS];
	/**
	* The handle of the headset.
	***/
	ovrHmd m_hHMD;
	/**
	* Eye render description (left).
	***/
	ovrEyeRenderDesc m_sEyeRenderDescLeft;
	/**
	* Eye render description (right).
	***/
	ovrEyeRenderDesc m_sEyeRenderDescRight;
	/**
	* The current pose of the rift.
	***/
	ovrPosef m_sPose;
	/**
	* The current rotation quaternion:
	***/
	OVR::Quat<float> m_sOrientation;
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
	* Render texture width (default).
	***/
	int m_nRenderTextureWidth;
	/**
	* Render texture height (default).
	***/
	int m_nRenderTextureHeight;
	/**
	* The control bitmap.
	***/
	HBITMAP m_hBitmapControl;
	/**
	* The control update bool.
	***/
	bool m_bControlUpdate;
	/**
	* The font used.
	***/
	HFONT m_hFont;
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
	* The index buffer for the left distortion mesh.
	***/
	LPDIRECT3DINDEXBUFFER9 m_pcDistortionIndexBufferLeft;
	/**
	* The index buffer for the right distortion mesh.
	***/
	LPDIRECT3DINDEXBUFFER9 m_pcDistortionIndexBufferRight;
	/**
	* Default FOV projection matrix.
	***/
	D3DMATRIX m_sDefaultFOVMatrixProjLeft;
	/**
	* Default FOV projection matrix.
	***/
	D3DMATRIX m_sDefaultFOVMatrixProjRight;
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	OculusTracker* pOculusTracker = new OculusTracker();
	return static_cast<AQU_Nodus*>(pOculusTracker);
}
