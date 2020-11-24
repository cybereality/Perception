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
#ifndef NOD_BASIC_CLASS
#define NOD_BASIC_CLASS

#define AQUILINUS_FONT_PIXELSIZE 128
#define AQUILINUS_FULL_TEXT_SIZE 1.0f
#define AQUILINUS_MEDIUM_TEXT_SIZE 0.38f
#define AQUILINUS_SMALL_TEXT_SIZE 0.25f
#define AQUILINUS_SMALLER_TEXT_SIZE 0.215f
#define AQUILINUS_TINY_TEXT_SIZE 0.15f

#include <ddraw.h>
#include <Windowsx.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <typeinfo>
#include "AQU_NodesStructures.h"
#include "..\dependecies\imgui\imgui.h"

/**
* Simple clipboard text helper.
***/
inline std::string GetClipboardText()
{
	// Try opening the clipboard
	if (!OpenClipboard(nullptr))
		return std::string();

	// Get handle of clipboard object for ANSI text
	HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData == nullptr)
		return std::string();

	// Lock the handle to get the actual text pointer
	char * pszText = static_cast<char*>(GlobalLock(hData));
	if (pszText == nullptr)
		return std::string();

	// Save text in a string class instance
	std::string text(pszText);

	// Release the lock
	GlobalUnlock(hData);

	// Release the clipboard
	CloseClipboard();

	return text;
}

/**
* The current control beahvior of a node.
***/
enum AQU_NodeBehavior
{
	Inactive,
	Translate,
	ConnectCommander,
	ConnectProvoker,
	WindowsEvent
};

/**
* Provoker/Invoker setting.
***/
enum class AQU_NodeProvokingType
{
	Both = 0,           /**< Node has both : Provoker and Invoker ***/
	OnlyProvoker = 1,   /**< Node has only Provoker, no Invoker ***/
	OnlyInvoker = 2,    /**< Node has no Provoker, only Invoker ***/
	None = 3            /**< Node has no Provoker and no Invoker ***/
};

/**
* Setting for the next provoking circle.
* To be set back to >DefaultBehavior< after next circle.
**/
enum class AQU_NextNodeCall
{
	DefaultBehavior = 0,  /** Default behavior : provoke -> call D3D method **/
	DoubleCall = 1,       /** Double call : call D3D method -> provoke -> call D3D method **/
};

/**
* Plugin flags.
**/
enum class AQU_PluginFlags
{
	DoubleCallFlag = 512,       /** Double call : call D3D method -> provoke -> call D3D method **/
	ImmediateReturnFlag = 1024, /** Immediate return : D3D method call replaced by plugin **/
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
	virtual UINT32          GetNodeTypeId() { return NULL; }
	virtual LPWSTR          GetCategory() { return L""; }
	virtual HBITMAP         GetLogo() { return nullptr; }
	virtual HBITMAP         GetControl() { return nullptr; }
	virtual DWORD           GetNodeWidth() { return 100; }
	virtual DWORD           GetNodeHeight() { return 100; }
	virtual int             GetProvokingType() { return 0; }
	virtual bool            GetMethodReplacement() { return false; }
	virtual DWORD           GetSaveDataSize() { return 0; }
	virtual char*           GetSaveData(UINT* pdwSizeOfData) { return nullptr; }
	virtual void            InitNodeData(char* pData, UINT dwSizeOfData) { (pData); (dwSizeOfData); }
	virtual DWORD           GetCommandersNumber() { return 0; }
	virtual DWORD           GetDecommandersNumber() { return 0; }
	virtual LPWSTR          GetCommanderName(DWORD dwCommanderIndex) { return L""; }
	virtual LPWSTR          GetDecommanderName(DWORD dwDecommanderIndex) { return L""; }
	virtual DWORD           GetCommanderType(DWORD dwCommanderIndex) { return 0; }
	virtual DWORD           GetDecommanderType(DWORD dwDecommanderIndex) { return 0; }
	virtual void*           GetOutputPointer(DWORD dwCommanderIndex) { return nullptr; }
	virtual void            SetInputPointer(DWORD dwDecommanderIndex, void* pData) { (dwDecommanderIndex); (pData); }
	virtual bool            SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)  { return false; }
	virtual void*           Provoke(void* pcThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)	{ (pcThis); (eD3D); (eD3DInterface); (eD3DMethod); (dwNumberConnected); (nProvokerIndex); return nullptr; }
	virtual void            WindowsEvent(UINT msg, WPARAM wParam, LPARAM lParam) {}
};

/**
* Aquilinus node prototype.
* Every Aquilinus node derives from this class. Note that GetNodeType() and GetNodeTypeId() MUST be overwritten in any derived class.
***/
class NOD_Basic
{
public:
	NOD_Basic(LONG nX, LONG nY, LONG nWidth, LONG nHeight);
	virtual ~NOD_Basic();

	/*** NOD_Basic public methods ***/
	virtual HRESULT          Translate(LONG nX, LONG nY, float fZoom);
	virtual HRESULT          Draw(POINT vcOrigin);
	virtual LONG*            GetCommanderConnectionIndices(std::vector<NOD_Basic*>* ppaNodes, DWORD dwIndex);
	virtual LONG*            GetProvokerConnectionIndices();
	virtual DWORD            GetCommanderConnectionsNumber(DWORD dwIndex);
	virtual DWORD            GetProvokerConnectionsNumber();
	virtual DWORD            GetInvokerConnectionsNumber();
	virtual void             SetNewIndex(DWORD unIndex);
	virtual AQU_NodeBehavior WindowsEvent(UINT msg, WPARAM wParam, LPARAM lParam);
	virtual bool             IsLocal(POINT vecCursor, POINT vcOrigin, float fZoom);
	virtual bool             SupportsD3DMethod(int eD3D, int eD3DInterface, int eD3DMethod);
	virtual POINT            GetConnectionOrigin();
	virtual void             ConnectCommander(NOD_Basic* pNode, LONG nThisNodeIndex);
	virtual void             ConnectDecommander(NOD_Basic* pNode, LONG nThisNodeIndex, DWORD dwCommanderIndex, DWORD dwDecommanderIndex);
	virtual void             ConnectProvoker(NOD_Basic* pNode, LONG nThisNodeIndex);
	virtual void             ConnectInvoker(NOD_Basic* pNode, LONG nDestNodeIndex);
	virtual void             AlignData(LONG nDecommanderIndex, void* pData);
	virtual void*            Provoke(void* pcThis, std::vector<NOD_Basic*>* ppaNodes) { return Provoke(pcThis, m_cProvoker.m_eD3D, m_cProvoker.m_eD3DInterface, m_cProvoker.m_eD3DMethod, ppaNodes); }
	virtual void*            Provoke(void* pcThis, int eD3D, int eD3DInterface, int eD3DMethod, std::vector<NOD_Basic*>* ppaNodes);
	virtual POINT            GetNodePosition();
	virtual ImVec2           GetNodeSize() { return ImVec2((float)m_vecSize.cx, (float)m_vecSize.cy); }
	virtual DWORD            GetSaveDataSize();
	virtual char*            GetSaveData(UINT* pdwSizeOfData);
	virtual void             InitNodeData(char* pData, UINT dwSizeOfData) { (pData); (dwSizeOfData); }
	virtual const char*      GetNodeType() { return typeid(this).name(); }
	virtual UINT             GetNodeTypeId() { return NULL; }
	virtual void             VerifyConnections(std::vector<NOD_Basic*>* ppaNodes);
	virtual AQU_NextNodeCall GetNextCycleBehavior() { AQU_NextNodeCall eRet = m_eNextNodeCall; m_eNextNodeCall = AQU_NextNodeCall::DefaultBehavior;	return eRet; }
	virtual bool             IsFirstDraw() { if (!m_bFirstDraw) { m_bFirstDraw = true; return false; } else return m_bFirstDraw; }

	/**
	* The title text of the node.
	**/
	LPCWSTR m_szTitle;
	/**
	* The title as std::string(). Converted within Draw()
	***/
	std::string m_acTitleA;
	/**
	* The node provoker.
	* Must be public since the detour classes will use it.
	***/
	NOD_Provoker m_cProvoker;
	/**
	* Vector to all node commanders.
	* Must be public since the detour classes will use it.
	***/
	std::vector<NOD_Commander*> m_paCommanders;
	/**
	* The return value for this node.
	* The type of the return value is determined by the node type.
	* To be changed in the Provoke() method.
	***/
	static void* m_pvReturn;
	/**
	* True if that node replaces the provoking node's return value;
	***/
	bool m_bReturn;
	/**
	* Node behavior for the next provoking circle.
	* Set back to >DefaultBehavior< after next call.
	**/
	AQU_NextNodeCall m_eNextNodeCall;
	/**
	* The provoker/invoker setting of this node.
	***/
	AQU_NodeProvokingType m_eNodeProvokingType;
	/**
	* Vector to all node decommanders.
	***/
	std::vector<NOD_Decommander*> m_paDecommanders;

protected:
	/**
	* Node invoker.
	***/
	NOD_Invoker m_cInvoker;
	/**
	* The position of the node in the working area (in full zoom pixel space).
	***/
	POINT m_vecPos;
	/**
	* The actual, floating x position.
	***/
	float m_fXPos;
	/**
	* The actual, floating y position.
	***/
	float m_fYPos;
	/**
	* The size of the node in the working area (in full zoom pixel space).
	**/
	SIZE m_vecSize;
	/**
	* Current zoom factor, needed for node translation.
	***/
	float m_fZoom;
	/**
	* Current local mouse cursor position.
	***/
	POINT m_vecLocalMouseCursor;
	/**
	* The mouse cursor origin for all mouse actions (in desktop pixel space).
	***/
	POINT m_vecMouseOrigin;
	/**
	* The behavior of the node.
	***/
	AQU_NodeBehavior m_NodeBehavior;
	/**
	* The last chosen commander index.
	***/
	int m_nCommanderIndex;
	/**
	* The connection origin to draw a new connection.
	***/
	POINT m_vecConnectionOrigin;
	/**
	* True if the last connected invoker is to be deleted within the next Provoke() call.
	* This must be done within the Provoke() call since this is the only safe way.
	* Otherwise the provoking circle could end up nowhere causing a crash.
	***/
	bool m_bPopBackConnectedInvokers;
	/**
	* Node index to be verified.
	* -1 if no node to be verified.
	***/
	int m_nNodeIndexToVerify;
	/**
	* Size of the node header.
	* If the mouse is within the border of the header, the user
	* can move the node, if not this will be handled as window event.
	***/
	DWORD m_dwHeaderSize;
	/*
	* True after that node is drawn the first time.
	**/
	bool m_bFirstDraw;
};

#endif