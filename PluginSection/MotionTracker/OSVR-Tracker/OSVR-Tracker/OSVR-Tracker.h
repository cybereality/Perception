/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

OSVR Tracker - Open Source Virtual Reality Tracking Node
Copyright (C) 2016 Denis Reischl

File <OSVR-Tracker.h> and
Class <OSVR-Tracker> :
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
#include<stdio.h>
#include<sstream>

#include<Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <osvr/ClientKit/ContextC.h>
#include <osvr/ClientKit/InterfaceC.h>
#include <osvr/ClientKit/InterfaceStateC.h>
#pragma comment(lib, "osvrClientKit.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include "..\..\..\Include\Vireio_Node_Plugtypes.h"

#define NUMBER_OF_COMMANDERS                          15

#define FLOAT_PI                            (3.1415926f)

/**
* Node Commander Enumeration.
***/
enum OSVR_Commanders
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
	View,
	ProjectionLeft,
	ProjectionRight,
	TargetWidth,
	TargetHeight,
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
class OSVR_Tracker : public AQU_Nodus
{
public:
	OSVR_Tracker();
	virtual ~OSVR_Tracker();

	/*** AQU_Nodus public methods ***/
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
	* The OSVR client context handle.
	***/
	OSVR_ClientContext m_psOSVR_ClientContext;
	/**
	* The OSVR client interface handle.
	***/
	OSVR_ClientInterface m_psOSVR_ClientInterface;
	/**
	* The OSVR pose state.
	* Contains both position and orientation.
	***/
	OSVR_PoseState m_sState;
	/**
	* The OSVR pose state time stamp.
	***/
	OSVR_TimeValue m_sTimestamp;
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
	* Euler angles. (0-Pitch, 1-Yaw, 2-Roll)
	***/
	float m_afEuler[3];
	/**
	* Euler angles predicted.
	***/
	float m_afEulerPredicted[3];
	/**
	* Position predicted.
	***/
	float m_afPositionPredicted[3];
	/**
	* Euler angles velocity.
	***/
	float m_afEulerVelocity[3];
	/**
	* Position velocity.
	***/
	float m_afPositionVelocity[3];
	/**
	* Position (float).
	***/
	float m_afPosition[3];
	/**
	* Position origin (float).
	***/
	float m_afPositionOrigin[3];
	/**
	* Game timer class for predicted tracking.
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
	/**
	* Recommended render target size.
	***/
	struct OSVR_Size
	{
		uint32_t unWidth;
		uint32_t unHeight;
	} m_sTargetSize;
	/**
	* Key code for hotkey >Sync HMD Yaw<.
	***/
	UINT m_nHotkeySync;
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	OSVR_Tracker* pOSVR_Tracker = new OSVR_Tracker();
	return static_cast<AQU_Nodus*>(pOSVR_Tracker);
}
