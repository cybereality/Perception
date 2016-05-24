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
#include<sstream>

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include "OVR_CAPI_D3D.h"
#include "..\..\..\Include\Vireio_Node_Plugtypes.h"

#define NUMBER_OF_COMMANDERS                          25

#define MATH_FLOAT_PI                (3.1415926f)
#define MATH_FLOAT_TWOPI             (2.0f *MATH_FLOAT_PI)
#define MATH_FLOAT_PIOVER2           (0.5f *MATH_FLOAT_PI)
#define MATH_FLOAT_PIOVER4           (0.25f*MATH_FLOAT_PI)
#define MATH_FLOAT_E                 (2.7182818f)
#define MATH_FLOAT_MAXVALUE			 (FLT_MAX) 
#define MATH_FLOAT MINPOSITIVEVALUE  (FLT_MIN)  
#define MATH_FLOAT_RADTODEGREEFACTOR (360.0f / MATH_FLOAT_TWOPI)
#define MATH_FLOAT_DEGREETORADFACTOR (MATH_FLOAT_TWOPI / 360.0f)
#define MATH_FLOAT_TOLERANCE		 (0.00001f)
#define MATH_FLOAT_SINGULARITYRADIUS (0.0000001f) // Use for Gimbal lock numerical problems

#define MATH_DOUBLE_PI                (3.14159265358979)
#define MATH_DOUBLE_TWOPI             (2.0f *MATH_DOUBLE_PI)
#define MATH_DOUBLE_PIOVER2           (0.5f *MATH_DOUBLE_PI)
#define MATH_DOUBLE_PIOVER4           (0.25f*MATH_DOUBLE_PI)
#define MATH_DOUBLE_E                 (2.71828182845905)
#define MATH_DOUBLE_MAXVALUE		  (DBL_MAX)
#define MATH_DOUBLE MINPOSITIVEVALUE  (DBL_MIN)
#define MATH_DOUBLE_RADTODEGREEFACTOR (360.0f / MATH_DOUBLE_TWOPI)
#define MATH_DOUBLE_DEGREETORADFACTOR (MATH_DOUBLE_TWOPI / 360.0f)
#define MATH_DOUBLE_TOLERANCE		  (0.00001)
#define MATH_DOUBLE_SINGULARITYRADIUS (0.000000000001) // Use for Gimbal lock numerical problems

/**
* Definitions of axes for coordinate and rotation conversions.
* LibOVR 0.4.x enumeration.
***/
enum Axis
{
	Axis_X = 0, Axis_Y = 1, Axis_Z = 2
};

/**
* RotateDirection describes the rotation direction around an axis, interpreted as follows:
*  CW  - Clockwise while looking "down" from positive axis towards the origin.
*  CCW - Counter-clockwise while looking from the positive axis towards the origin,
*        which is in the negative axis direction.
*  CCW is the default for the RHS coordinate system. Oculus standard RHS coordinate
*  system defines Y up, X right, and Z back (pointing out from the screen). In this
*  system Rotate_CCW around Z will specifies counter-clockwise rotation in XY plane.
* LibOVR 0.4.x enumeration.
***/
enum RotateDirection
{
	Rotate_CCW = 1,
	Rotate_CW  = -1 
};

/**
* Constants for right handed and left handed coordinate systems
* LibOVR 0.4.x enumeration.
***/
enum HandedSystem
{
	Handed_R = 1, Handed_L = -1
};

/**
* AxisDirection describes which way the coordinate axis points. Used by WorldAxes.
* LibOVR 0.4.x enumeration.
***/
enum AxisDirection
{
	Axis_Up    =  2,
	Axis_Down  = -2,
	Axis_Right =  1,
	Axis_Left  = -1,
	Axis_In    =  3,
	Axis_Out   = -3
};

/**
* GetEulerAngles extracts Euler angles from the quaternion, in the specified order of
* axis rotations and the specified coordinate system. Right-handed coordinate system
* is the default, with CCW rotations while looking in the negative axis direction.
* Here a,b,c, are the Yaw/Pitch/Roll angles to be returned.
* rotation a around axis A1
* is followed by rotation b around axis A2
* is followed by rotation c around axis A3
* rotations are CCW or CW (D) in LH or RH coordinate system (S)
* (LibOVR 0.4.x method)
***/
struct __ovrQuatf : public ovrQuatf
{
	//float x, y, z, w;
	template <Axis A1, Axis A2, Axis A3, RotateDirection D, HandedSystem S>
	void GetEulerAngles(float *a, float *b, float *c) const 
	{
		static_assert((A1 != A2) && (A2 != A3) && (A1 != A3), "(A1 != A2) && (A2 != A3) && (A1 != A3)");

		float Q[3] = { x, y, z };  //Quaternion components x,y,z

		float ww  = w*w;
		float Q11 = Q[A1]*Q[A1];
		float Q22 = Q[A2]*Q[A2];
		float Q33 = Q[A3]*Q[A3];

		float psign = float(-1);
		// Determine whether even permutation
		if (((A1 + 1) % 3 == A2) && ((A2 + 1) % 3 == A3))
			psign = float(1);

		float s2 = psign * float(2) * (psign*w*Q[A2] + Q[A1]*Q[A3]);

		if (s2 < float(-1) + ((float)MATH_DOUBLE_SINGULARITYRADIUS))
		{ // South pole singularity
			*a = float(0);
			*b = -S*D*((float)MATH_DOUBLE_PIOVER2);
			*c = S*D*atan2(float(2)*(psign*Q[A1]*Q[A2] + w*Q[A3]),
				ww + Q22 - Q11 - Q33 );
		}
		else if (s2 > float(1) - ((float)MATH_DOUBLE_SINGULARITYRADIUS))
		{  // North pole singularity
			*a = float(0);
			*b = S*D*((float)MATH_DOUBLE_PIOVER2);
			*c = S*D*atan2(float(2)*(psign*Q[A1]*Q[A2] + w*Q[A3]),
				ww + Q22 - Q11 - Q33);
		}
		else
		{
			*a = -S*D*atan2(float(-2)*(w*Q[A1] - psign*Q[A2]*Q[A3]),
				ww + Q33 - Q11 - Q22);
			*b = S*D*asin(s2);
			*c = S*D*atan2(float(2)*(w*Q[A3] - psign*Q[A1]*Q[A2]),
				ww + Q11 - Q22 - Q33);
		}      
		return;
	}
};

/**
* Node Commander Enumeration.
***/
enum OTR_Commanders
{
	Pitch,
	Yaw,
	Roll,
	OrientationW,
	OrientationX,
	OrientationY,
	OrientationZ,
	PositionX,
	PositionY,
	PositionZ,
	OBSOLETE_CameraFrustumHFovInRadians,  /**<  The horizontal FOV of the position tracking camera frustum. **/
	OBSOLETE_CameraFrustumVFovInRadians,  /**<  The vertical FOV of the position tracking camera frustum. **/
	OBSOLETE_CameraFrustumNearZInMeters,  /**<  The distance from the position tracking camera to the near frustum bounds. **/
	OBSOLETE_CameraFrustumFarZInMeters,   /**<  The distance from the position tracking camera to the far frustum bounds. **/
	HmdCaps,                     /**<  HMD capability bits described by ovrHmdCaps. **/
	TrackingCaps,                /**<  Tracking capability bits describing whether orientation, position tracking, and yaw drift correction are supported. **/
	ResolutionW,                 /**<  Device Resolution Width. **/
	ResolutionH,                 /**<  Device Resolution Height. **/
	DefaultEyeFovLeft,           /**<  Recommended optical ﬁeld of view for each eye. (Left) **/
	DefaultEyeFovRight,          /**<  Recommended optical ﬁeld of view for each eye. (Right) **/
	MaxEyeFovLeft,               /**<  Maximum optical ﬁeld of view that can be practically rendered for each eye. (Left) **/
	MaxEyeFovRight,              /**<  Maximum optical ﬁeld of view that can be practically rendered for each eye. (Right) **/
	DefaultProjectionMatrixLeft, /**<  Default FOV Oculus Rift projection matrix (Left) **/
	DefaultProjectionMatrixRight,/**<  Default FOV Oculus Rift projection matrix (Left) **/
	HMD_Handle,                  /**<  Handle to the HMD, to be connected with the Oculus Direct Mode Node. ***/
};

/**
* Oculus Tracker Node Plugin.
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
	ovrSession m_hHMD;
	/**
	* The HMD description. 
	***/
	ovrHmdDesc m_sHMDDesc;
	/**
	* Adapter Identifier.
	***/
	ovrGraphicsLuid m_sLuid;
	/**
	* The current pose of the rift.
	***/
	ovrPosef m_sPose;
	/**
	* The current rotation quaternion:
	***/
	__ovrQuatf m_sOrientation;
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
