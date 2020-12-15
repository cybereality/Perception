/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Stereo Splitter - Vireio Perception Render Target Handler
Copyright (C) 2015 Denis Reischl

File <AQU_Nodus.h> and
Class <AQU_Nodus> :
Copyright (C) 2015 Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 onwards 2014 by Grant Bagwell, Simon Brown and Neil Schneider
v4.0.x 2015 by Denis Reischl, Grant Bagwell, Simon Brown and Neil Schneider

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

#include<Windows.h>
#include<typeinfo>

/**
* DirectX version enumeration.
* Do not change this declaration to ensure proper functionality.
***/
enum AQU_DirectXVersion
{
	DirectX_8_0    = 80,
	DirectX_8_0a   = 81,	
	DirectX_8_1    = 82,
	DirectX_8_1a   = 83,
	DirectX_8_1b   = 84,
	DirectX_8_2    = 85,
	DirectX_9_0    = 90,
	DirectX_9_0a   = 91,
	DirectX_9_0b   = 92,
	DirectX_9_0c   = 93,
	DirectX_9_29   = 94,
	DirectX_10     = 100,
	DirectX_10_1   = 101,
	DirectX_11     = 110,
	DirectX_11_1   = 111,
	DirectX_11_2   = 112,
	DirectX_12     = 120
};

/**
* Aquilinus plugin import node.
* All methods empty.
***/
class AQU_Nodus
{
public:
	AQU_Nodus() {}
	~AQU_Nodus() {}


	/*** AQU_Nodus public methods ***/
	virtual const char*     GetNodeType() { return typeid(this).name(); }
	virtual UINT            GetNodeTypeId() { return NULL; }
	virtual LPWSTR          GetCategory() { return L""; }
	virtual HBITMAP         GetLogo() { return nullptr; }
	virtual HBITMAP         GetControl() { return nullptr; }
	virtual DWORD           GetNodeWidth() { return 100; }
	virtual DWORD           GetNodeHeight() { return 100; }
	virtual int             GetProvokingType() { return 0; }
	virtual bool            GetMethodReplacement() { return false; }
	virtual DWORD           GetSaveDataSize() { return 0; }
	virtual char*           GetSaveData(UINT* punSizeOfData) { return nullptr; }
	virtual void            InitNodeData(char* pData, UINT unSizeOfData) { (pData); (unSizeOfData); }
	virtual DWORD           GetCommandersNumber() { return 0; }
	virtual DWORD           GetDecommandersNumber() { return 0; }
	virtual LPWSTR          GetCommanderName(DWORD unCommanderIndex) { return L""; }
	virtual LPWSTR          GetDecommanderName(DWORD unDecommanderIndex) { return L""; }
	virtual DWORD           GetCommanderType(DWORD unCommanderIndex) { return 0; }
	virtual DWORD           GetDecommanderType(DWORD unDecommanderIndex) { return 0; }
	virtual void*           GetOutputPointer(DWORD unCommanderIndex) { return nullptr; }
	virtual void            SetInputPointer(DWORD unDecommanderIndex, void* pData) { (unDecommanderIndex); (pData); }
	virtual bool            SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)  { return false; }
	virtual void*           Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD unNumberConnected, int& nProvokerIndex)	{ (pThis); (eD3D); (eD3DInterface); (eD3DMethod); (unNumberConnected); (nProvokerIndex); return nullptr; }
	virtual void            WindowsEvent(UINT msg, WPARAM wParam, LPARAM lParam) {}
};
