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

#include <winsock2.h>						// include first or will conflict with #include<windows.h> in MotionTracker.h

#include "SocketTracker.h"

#include <ws2tcpip.h>						// for getaddrinfo
#include <iphlpapi.h>						// for addrinfo
#include <iostream>
#include <sstream>							// for stringstream

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 49015

SocketTracker::SocketTracker(void):MotionTracker()
{
	OutputDebugString("Socket Tracker Created\n");
	init();
}

SocketTracker::~SocketTracker(void)
{
	CloseHandle(m_hThread);									// close thread

	closesocket(ListenSocket);
	closesocket(ClientSocket);

	WSACleanup();
}

int SocketTracker::init()
{
	OutputDebugString("Socket Tracker Init\n");

	// add init code
    m_dwThreadID = 0;
    m_hThread    = 0;

	ListenSocket = INVALID_SOCKET;
	ClientSocket = INVALID_SOCKET;

	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);		// starts Winsock version 2.2 and initializes wsaData

	if(result != 0)											// Winsock failed to launch
		return false;
	
	if(wsaData.wVersion != MAKEWORD(2, 2))					// winsock is not version 2.2
	{
		WSACleanup();
		return false;
	}

	// change to support other ports
	return ListenOnPort(DEFAULT_PORT);
}


int SocketTracker::getOrientation(float* yaw, float* pitch, float* roll) 
{
	OutputDebugString("Socket Tracker getOrient\n");

	*roll = this->roll;
	*pitch = this->pitch;
	*yaw = this->yaw;
	return 0; 
}

bool SocketTracker::isAvailable()
{
	// change to connection status
	return true;
}

void SocketTracker::updateOrientation()
{
	OutputDebugString("Motion Tracker updateOrientation\n");

	// need to add multipliers...

	if(getOrientation(&yaw, &pitch, &roll) == 0)
	{
		deltaYaw = yaw - currentYaw;
		deltaPitch = pitch - currentPitch;

		mouseData.mi.dx = (long)(deltaYaw);
		mouseData.mi.dy = (long)(deltaPitch);
		
		OutputDebugString("Motion Tracker SendInput\n");
		SendInput(1, &mouseData, sizeof(INPUT));
		currentYaw = yaw;
		currentPitch = pitch;
		currentRoll = roll;
	}
}

bool SocketTracker::ListenOnPort(int portNum)
{
	// convert int portNum to string
	std::stringstream ss;
	ss << portNum;
	port =  ss.str();
	
	return CreateMsgThread();			// start helper thread that handles socket communication
}

bool SocketTracker::CreateMsgThread()
{
	if(m_hThread != 0)
		return false;					// thread already exists
	
	m_hThread = CreateThread(0, 0, MsgThread, this, 0, &m_dwThreadID);
	if(!m_hThread)
		return false;					// Could not create thread
	return true;              
}



DWORD WINAPI SocketTracker::MsgThread(LPVOID pvParam)
{
	SocketTracker* sockTrackPtr = (SocketTracker*)pvParam;				// pointer to parent SocketTracker

	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	int iResult = 0;

	struct addrinfo hints;
	struct addrinfo *result = NULL;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, sockTrackPtr->port.c_str(), &hints, &result);
	if(iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	do {
		// Create a SOCKET for connecting to server
		sockTrackPtr->ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if(sockTrackPtr->ListenSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return 1;
		}

		// Setup the TCP listening socket
		iResult = bind( sockTrackPtr->ListenSocket, result->ai_addr, (int)result->ai_addrlen);
		if(iResult == SOCKET_ERROR) {
			printf("bind failed with error: %d\n", WSAGetLastError());
			freeaddrinfo(result);
			closesocket(sockTrackPtr->ListenSocket);
			WSACleanup();
			return 1;
		}


		//if socket is connected read data and update class
		iResult = listen(sockTrackPtr->ListenSocket, SOMAXCONN);
		if(iResult == SOCKET_ERROR) {
			printf("listen failed with error: %d\n", WSAGetLastError());
			closesocket(sockTrackPtr->ListenSocket);
			WSACleanup();
			return 1;
		}

		// Accept a client socket
		sockTrackPtr->ClientSocket = accept(sockTrackPtr->ListenSocket, NULL, NULL);
		if(sockTrackPtr->ClientSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(sockTrackPtr->ListenSocket);
			WSACleanup();
			return 1;
		}

		// No longer need server socket
		closesocket(sockTrackPtr->ListenSocket);

		// Receive until the peer shuts down the connection
		float tmp = 0;
		std::string tmpStr = "";
		int lastS = 0;
		int lastE = 0;

		// Receive until the peer shuts down the connection
		do {
			iResult = recv(sockTrackPtr->ClientSocket, recvbuf, recvbuflen, 0);
			if (iResult > 0) {
				recvbuf[iResult] = '\0';
				//format <VST><yaw>float</yaw><pitch>float</pitch><roll>float</roll></VST>
				std::string tmpSt(recvbuf);								// convert to string

				// read yaw
				lastS = tmpSt.rfind("<yaw>");
				lastS += 5;												// skip over tag
				lastE = tmpSt.rfind("</yaw>");
				lastE -= lastS;											// skip tag
				tmpStr = tmpSt.substr(lastS,lastE);
				if(lastS < 0 || lastE < 0 || lastS+lastE > iResult)
					continue;											// error reading tag
				sockTrackPtr->yaw = (float)atof(tmpStr.c_str());

				// read pitch
				lastS = tmpSt.rfind("<pitch>");
				lastS += 7;												// skip over tag
				lastE = tmpSt.rfind("</pitch>");
				lastE -= lastS;											// skip tag
				tmpStr = tmpSt.substr(lastS,lastE);
				if(lastS < 0 || lastE < 0 || lastS+lastE > iResult)
					continue;											// error reading tag
				sockTrackPtr->pitch = (float)atof(tmpStr.c_str());

				// read roll
				lastS = tmpSt.rfind("<roll>");
				lastS += 6;												// skip over tag
				lastE = tmpSt.rfind("</roll>");
				lastE -= lastS;											// skip tag
				tmpStr = tmpSt.substr(lastS,lastE);
				if(lastS < 0 || lastE < 0 || lastS+lastE > iResult)
					continue;											// error reading tag
				sockTrackPtr->roll = (float)atof(tmpStr.c_str());
			}

		} while (iResult > 0);											// while still receiving data from client

		closesocket(sockTrackPtr->ClientSocket);						// lost connection with client

	} while (true); // loop forever
}