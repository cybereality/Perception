/********************************************************************
Vireio Perception : Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Aquilinus : Vireio Perception 3D Modification Studio
Copyright © 2014 Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 to v3.0.x 2014-2015 by Grant Bagwell, Simon Brown and Neil Schneider
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

#include <Windows.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <typeinfo>
#include "..\dependecies\imgui\imgui.h"
#include "..\dependecies\imgui\imgui_internal.h"
#include "AQU_GlobalTypes.h"

/// <summary>
/// Aquilinus plugin import node.
/// All methods empty.
/// </summary>
class AQU_Nodus
{
public:
	AQU_Nodus(ImGuiContext* sCtx) { ImGui::SetCurrentContext(sCtx); }
	~AQU_Nodus() {}

	/*** AQU_Nodus public methods ***/
	virtual     const char* GetNodeType() { return typeid(this).name(); }
	virtual          UINT32 GetNodeTypeId() { return NULL; }
	virtual          LPWSTR GetCategory() { return L""; }
	virtual         HBITMAP GetLogo() { return nullptr; }
	virtual          ImVec2 GetNodeSize() { return ImVec2(100.f, 100.f); }
	virtual             int GetProvokingType() { return 0; }
	virtual            bool GetMethodReplacement() { return false; }
	virtual           DWORD GetSaveDataSize() { return 0; }
	virtual           char* GetSaveData(UINT* pdwSizeOfData) { return nullptr; }
	virtual            void InitNodeData(char* pData, UINT dwSizeOfData) { (pData); (dwSizeOfData); }
	virtual           DWORD GetCommandersNumber() { return 0; }
	virtual           DWORD GetDecommandersNumber() { return 0; }
	virtual          LPWSTR GetCommanderName(DWORD dwCommanderIndex) { return L""; }
	virtual          LPWSTR GetDecommanderName(DWORD dwDecommanderIndex) { return L""; }
	virtual           DWORD GetCommanderType(DWORD dwCommanderIndex) { return 0; }
	virtual           DWORD GetDecommanderType(DWORD dwDecommanderIndex) { return 0; }
	virtual           void* GetOutputPointer(DWORD dwCommanderIndex) { return nullptr; }
	virtual            void SetInputPointer(DWORD dwDecommanderIndex, void* pData) { (dwDecommanderIndex); (pData); }
	virtual            bool SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod) { return false; }
	virtual           void* Provoke(void* pcThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex) { (pcThis); (eD3D); (eD3DInterface); (eD3DMethod); (dwNumberConnected); (nProvokerIndex); return nullptr; }
	virtual            void UpdateImGuiControl(float fZoom) { return; }
};
