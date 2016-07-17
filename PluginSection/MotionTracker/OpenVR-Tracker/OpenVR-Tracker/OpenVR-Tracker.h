/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

OpenVR Tracker - Open Virtual Reality Tracking Node
Copyright (C) 2016 Denis Reischl

File <OpenVR-Tracker.h> and
Class <OpenVR-Tracker> :
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

#include"AQU_Nodus.h"
#include"Resources.h"
#include<stdio.h>
#include<sstream>

#include <openvr.h>
#pragma comment(lib, "openvr_api.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include "..\..\..\Include\Vireio_Node_Plugtypes.h"

#define PNT_FLOAT_PLUG_TYPE                          104
#define PNT_INT_PLUG_TYPE                            107 
#define PNT_UINT_PLUG_TYPE                           112

#define NUMBER_OF_COMMANDERS                          14

#define FLOAT_PI                            (3.1415926f)

/**
* Matrix helper DX.
* @returns Left handed D3D matrix from Steam HmdMatrix34_t type.
***/
D3DMATRIX GetLH(vr::HmdMatrix34_t sMat)
{
	D3DXMATRIX sRet = D3DXMATRIX(
		sMat.m[0][0], sMat.m[1][0], -sMat.m[2][0], 0.0,
		sMat.m[0][1], sMat.m[1][1], -sMat.m[2][1], 0.0,
		-sMat.m[0][2], -sMat.m[1][2], sMat.m[2][2], 0.0,
		sMat.m[0][3], sMat.m[1][3], -sMat.m[2][3], 1.0f
		);

	return sRet;
}
/**
* Matrix helper DX.
* @returns Left handed D3D matrix from Steam HmdMatrix44_t type.
***/
D3DMATRIX GetLH(vr::HmdMatrix44_t sMat)
{
	D3DXMATRIX sRet = D3DXMATRIX(
		sMat.m[0][0], sMat.m[1][0], -sMat.m[2][0], sMat.m[3][0],
		sMat.m[0][1], sMat.m[1][1], -sMat.m[2][1], sMat.m[3][1],
		-sMat.m[0][2], -sMat.m[1][2], sMat.m[2][2], sMat.m[3][2],
		sMat.m[0][3], sMat.m[1][3], -sMat.m[2][3], sMat.m[3][3]
		);

	return sRet;
}

/**
* Matrix helper DX.
* @returns Left handed OpenVR projection matrix for specified eye.
***/
D3DXMATRIX GetHMDMatrixProjectionEyeLH(vr::IVRSystem* pHmd, vr::Hmd_Eye nEye, /*float fFOVy, float fAspect, */float fNearClip, float fFarClip)
{
	if (!pHmd)
		return D3DXMATRIX();

	// get raw projection data
	float fLeft, fRight, fTop, fBottom;
	pHmd->GetProjectionRaw(nEye, &fLeft, &fRight, &fTop, &fBottom);

	// create matrix
	D3DXMATRIX sRet;
	D3DXVECTOR2 asVec[2];
	asVec[0].x = fLeft;
	asVec[0].y = fRight;
	asVec[1].x = -fBottom;
	asVec[1].y = -fTop;
	asVec[0] *= fNearClip;
	asVec[1] *= fNearClip;
	D3DXMatrixPerspectiveOffCenterLH(&sRet, asVec[0].x, asVec[0].y, asVec[1].x, asVec[1].y, fNearClip, fFarClip);

	return sRet;
}
/**
* Matrix helper DX.
* @returns Left handed OpenVR pose matrix for specified eye.
***/
D3DXMATRIX GetHMDMatrixPoseEyeLH(vr::IVRSystem* pHmd, vr::Hmd_Eye nEye)
{
	if (!pHmd)
		return D3DXMATRIX();

	vr::HmdMatrix34_t sMat = pHmd->GetEyeToHeadTransform(nEye);
	D3DXMATRIX matrixObj = D3DXMATRIX(
		sMat.m[0][0], sMat.m[1][0], sMat.m[2][0], 0.0,
		sMat.m[0][1], sMat.m[1][1], sMat.m[2][1], 0.0,
		sMat.m[0][2], sMat.m[1][2], sMat.m[2][2], 0.0,
		sMat.m[0][3], sMat.m[1][3], sMat.m[2][3], 1.0f
		);

	D3DXMATRIX matrixObjInv;
	D3DXMatrixInverse(&matrixObjInv, 0, &matrixObj);
	return matrixObjInv;
}

/**
* Node Commander Enumeration.
***/
enum OpenVR_Commanders
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
	IVRSystem,
	View,
	ProjectionLeft,
	ProjectionRight,
};

/**
* Game timer helper class for OpenVR prediction model.
* Based on class <GameTimer> by Frank Luna (C) 2011.
***/
class Vireio_GameTimer
{
public:
	Vireio_GameTimer() : m_dSecondsPerCount(0.0), m_dDeltaTime(-1.0), m_nBaseTime(0), m_nPausedTime(0), m_nStopTime(0), m_nPreviousTime(0), m_nCurrentTime(0), m_bStopped(false)
	{
		INT64 nCountsPerSec;
		QueryPerformanceFrequency((LARGE_INTEGER*)&nCountsPerSec);
		m_dSecondsPerCount = 1.0 / (double)nCountsPerSec;
	}

	/**
	* Total time from game start, in seconds.
	***/
	float TotalTime() const
	{
		if (m_bStopped) { return (float)(((m_nStopTime - m_nPausedTime) - m_nBaseTime)*m_dSecondsPerCount); }
		else { return (float)(((m_nCurrentTime - m_nPausedTime) - m_nBaseTime)*m_dSecondsPerCount); }
	}
	/**
	* Time since last frame, in seconds.
	***/
	float DeltaTime() const
	{
		return (float)m_dDeltaTime;
	}
	/**
	* Sets all values to zero.
	***/
	void Reset()
	{
		INT64 nCurrentTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&nCurrentTime);

		m_nBaseTime = nCurrentTime;
		m_nPreviousTime = nCurrentTime;
		m_nStopTime = 0;
		m_bStopped = false;
	}
	/**
	* Restart after pause.
	***/
	void Start()
	{
		INT64 nStartTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&nStartTime);

		if (m_bStopped)
		{
			m_nPausedTime += (nStartTime - m_nStopTime);

			m_nPreviousTime = nStartTime;
			m_nStopTime = 0;
			m_bStopped = false;
		}
	}
	/**
	* Pause this class.
	***/
	void Stop()
	{
		if (!m_bStopped)
		{
			INT64 nCurrentTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&nCurrentTime);

			m_nStopTime = nCurrentTime;
			m_bStopped = true;
		}
	}
	/**
	* Call this once per frame.
	***/
	void Tick()
	{
		if (m_bStopped)
		{
			m_dDeltaTime = 0.0;
			return;
		}

		INT64 nCurrentTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&nCurrentTime);
		m_nCurrentTime = nCurrentTime;

		// Time difference between this frame and the previous.
		m_dDeltaTime = (m_nCurrentTime - m_nPreviousTime)*m_dSecondsPerCount;

		// Prepare for next frame.
		m_nPreviousTime = m_nCurrentTime;

		// Force nonnegative.
		if (m_dDeltaTime < 0.0)
		{
			m_dDeltaTime = 0.0;
		}
	}

private:
	double m_dSecondsPerCount;
	double m_dDeltaTime;

	INT64 m_nBaseTime;
	INT64 m_nPausedTime;
	INT64 m_nStopTime;
	INT64 m_nPreviousTime;
	INT64 m_nCurrentTime;

	bool m_bStopped;
};

/**
* Vireio Open Source VR Tracker Node Plugin.
***/
class OpenVR_Tracker : public AQU_Nodus
{
public:
	OpenVR_Tracker();
	virtual ~OpenVR_Tracker();

	/*** AQU_Nodus public methods ***/
	virtual const char*     GetNodeType();
	virtual UINT            GetNodeTypeId();
	virtual LPWSTR          GetCategory();
	virtual HBITMAP         GetLogo();
	virtual HBITMAP         GetControl();
	virtual DWORD           GetNodeWidth() { return 4 + 256 + 4; }
	virtual DWORD           GetNodeHeight() { return 128; }
	virtual DWORD           GetCommandersNumber() { return NUMBER_OF_COMMANDERS; }
	virtual LPWSTR          GetCommanderName(DWORD dwCommanderIndex);
	virtual DWORD           GetCommanderType(DWORD dwCommanderIndex);
	virtual void*           GetOutputPointer(DWORD dwCommanderIndex);
	virtual bool            SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod);
	virtual void*           Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex);

private:
	/*** OpenVR_Tracker private methods ***/
	void ProcessVREvent(const vr::VREvent_t & event);

	/**
	* OpenVR system.
	***/
	vr::IVRSystem *m_pHMD;
	/**
	* Tracking system name.
	***/
	std::string m_strDriver;
	/**
	* Serial number.
	***/
	std::string m_strDisplay;
	/**
	* The (tracked) device poses (for all devices).
	***/
	vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	/**
	* Euler angles (for all devices).
	***/
	D3DXVECTOR3 m_sEuler[vr::k_unMaxTrackedDeviceCount];
	/**
	* Orientation (for all devices)
	***/
	D3DXQUATERNION m_sOrientation[vr::k_unMaxTrackedDeviceCount];
	/**
	* Position (for all devices).
	***/
	D3DXVECTOR3 m_sPosition[vr::k_unMaxTrackedDeviceCount];
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
	* Game timer class for prediction model.
	***/
	Vireio_GameTimer m_cGameTimer;
	/**
	* Current view matrix.
	***/
	D3DXMATRIX m_sView;
	/**
	* Current projection matrix left/right.
	* Inherits "to eye" translation.
	***/
	D3DXMATRIX m_asProjection[2];
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	OpenVR_Tracker* pOpenVR_Tracker = new OpenVR_Tracker();
	return static_cast<AQU_Nodus*>(pOpenVR_Tracker);
}
