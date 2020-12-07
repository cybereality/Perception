/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

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
#ifndef SOCKETTRACKER_H_INCLUDED
#define SOCKETTRACKER_H_INCLUDED


#include "MotionTracker.h"

#include <string>

class SocketTracker : public MotionTracker
{
public:
	SocketTracker(void);
	~SocketTracker(void);


	int getOrientation(float* yaw, float* pitch, float* roll);
	bool isAvailable();
	void updateOrientation();
	int init();

	bool ListenOnPort(int portNum);

private:
	SOCKET ListenSocket;
	SOCKET ClientSocket;

    DWORD m_dwThreadID;
    HANDLE m_hThread;

	std::string port;															// port number to listen on


	bool SocketTracker::CreateSockets();

	bool CreateMsgThread();
	static DWORD WINAPI MsgThread(LPVOID pvParam);
};


#endif