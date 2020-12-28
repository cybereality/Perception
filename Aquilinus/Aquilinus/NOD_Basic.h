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

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include <Windows.h>
#include <Windowsx.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <typeinfo>
#include "AQU_NodesStructures.h"
#include "AQU_Nodus.h"

/// <summary>
/// Simple clipboard text helper.
/// </summary>
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
	char* pszText = static_cast<char*>(GlobalLock(hData));
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

/// <summary>
/// The current control beahvior of a node.
/// </summary>
enum class AQU_NodeBehavior
{
	Inactive,
	Translate,
	ConnectCommander,
	ConnectProvoker
};

/// <summary>
/// Provoker/Invoker setting.
/// </summary>
enum class AQU_NodeProvokingType
{
	Both = 0,           /**< Node has both : Provoker and Invoker ***/
	OnlyProvoker = 1,   /**< Node has only Provoker, no Invoker ***/
	OnlyInvoker = 2,    /**< Node has no Provoker, only Invoker ***/
	None = 3            /**< Node has no Provoker and no Invoker ***/
};

/// <summary>
/// Setting for the next provoking circle.
/// To be set back to >DefaultBehavior< after next circle.
/// </summary>
enum class AQU_NextNodeCall
{
	DefaultBehavior = 0,  /** Default behavior : provoke -> call D3D method **/
	DoubleCall = 1,       /** Double call : call D3D method -> provoke -> call D3D method **/
};

/// <summary>
/// Plugin flags.
/// </summary>
enum class AQU_PluginFlags
{
	DoubleCallFlag = 512,       /** Double call : call D3D method -> provoke -> call D3D method **/
	ImmediateReturnFlag = 1024, /** Immediate return : D3D method call replaced by plugin **/
};

/// <summary>
/// Slot enumeration. (this needs c++17)
/// -2 -> Invoker
/// -1 -> Provoker
/// </summary>
enum struct Slot : int { };
constexpr Slot Slot_Invoker{ -2 };
constexpr Slot Slot_Provoker{ -1 };

/// <summary>
/// Aquilinus node prototype.
/// Every Aquilinus node derives from this class. Note that GetNodeType() and GetNodeTypeId() MUST be overwritten in any derived class.
/// </summary>
class NOD_Basic
{
public:
	NOD_Basic(LONG nX, LONG nY, LONG nWidth, LONG nHeight);
	virtual ~NOD_Basic();

	/*** NOD_Basic public methods ***/
	virtual HRESULT          Update(float fZoom);
	virtual LONG* GetCommanderConnectionIndices(std::vector<NOD_Basic*>* ppaNodes, DWORD dwIndex);
	virtual LONG* GetProvokerConnectionIndices();
	virtual DWORD            GetCommanderConnectionsNumber(DWORD dwIndex);
	virtual DWORD            GetProvokerConnectionsNumber();
	virtual DWORD            GetInvokerConnectionsNumber();
	virtual void             SetNewIndex(DWORD unIndex);
	virtual bool             SupportsD3DMethod(int eD3D, int eD3DInterface, int eD3DMethod);
	virtual void             InputSlot(Slot eIndex, float fZoom, float fOriginY);
	virtual void             OutputSlot(Slot eIndex, float fZoom, float fOriginY);
	virtual POINT            GetConnectionOrigin();
	virtual void             ConnectDecommander(NOD_Basic* pNode, LONG nDestNodeIndex, std::string acCommander, std::string acDecommander);
	virtual void             ConnectDecommander(NOD_Basic* pNode, LONG nDestNodeIndex, DWORD dwCommanderIndex, DWORD dwDecommanderIndex);
	virtual void             ConnectInvoker(NOD_Basic* pNode, LONG nDestNodeIndex);
	virtual void             AlignData(LONG nDecommanderIndex, void* pData);
	virtual void* Provoke(void* pcThis, std::vector<NOD_Basic*>* ppaNodes) { return Provoke(pcThis, m_cProvoker.m_eD3D, m_cProvoker.m_eD3DInterface, m_cProvoker.m_eD3DMethod, ppaNodes); }
	virtual void* Provoke(void* pcThis, int eD3D, int eD3DInterface, int eD3DMethod, std::vector<NOD_Basic*>* ppaNodes);
	virtual ImVec2           GetNodePosition() { return m_sPos; }
	virtual ImVec2           GetNodeSize() { return m_sSize; }
	virtual ImVec2           GetNodeHeaderTextSize();
	virtual float            GetSlotSpace() { return m_fSlotSpace; }
	virtual DWORD            GetSaveDataSize();
	virtual char* GetSaveData(UINT* pdwSizeOfData);
	virtual void             InitNodeData(char* pData, UINT dwSizeOfData) { (pData); (dwSizeOfData); }
	virtual const char* GetNodeType() { return typeid(this).name(); }
	virtual UINT             GetNodeTypeId() { return NULL; }
	virtual void             VerifyConnections(std::vector<NOD_Basic*>* ppaNodes);
	virtual AQU_NextNodeCall GetNextCycleBehavior() { AQU_NextNodeCall eRet = m_eNextNodeCall; m_eNextNodeCall = AQU_NextNodeCall::DefaultBehavior;	return eRet; }
	virtual bool             IsFirstDraw() { if (!m_bFirstDraw) { m_bFirstDraw = true; return false; } else return m_bFirstDraw; }
	virtual bool             HasInvoker() { return ((m_eNodeProvokingType == AQU_NodeProvokingType::Both) || (m_eNodeProvokingType == AQU_NodeProvokingType::OnlyInvoker)); }
	virtual bool             HasProvoker() { return ((m_eNodeProvokingType == AQU_NodeProvokingType::Both) || (m_eNodeProvokingType == AQU_NodeProvokingType::OnlyProvoker)); }
	virtual void             ConvertStrings();

	/// <summary>
	/// The title text of the node.
	/// </summary>
	LPCWSTR m_szTitle;
	/// <summary>
	/// The title as std::string(). Converted within Draw()
	/// </summary>
	std::string m_acTitleA;
	/// <summary>
	/// The actual, floating position in the canvas. Public, since accessed by ImNodes.
	/// </summary>
	ImVec2 m_sPos;
	/// <summary>
	/// True if active.
	/// </summary>
	bool m_bActive;
	/// <summary>
	/// The node provoker.
	/// Must be public since the detour classes will use it.
	/// </summary>
	NOD_Provoker m_cProvoker;
	/// <summary>
	/// Vector to all node commanders.
	/// Must be public since the detour classes will use it.
	/// </summary>
	std::vector<NOD_Commander*> m_paCommanders;
	/// <summary>
	/// Vector to all temporary node commanders.
	/// Must be public since the detour classes will use it.
	/// Temporary commanders can be bunched to single commanders.
	/// </summary>
	std::vector<NOD_Commander*> m_paCommandersTemporary;
	/// <summary>
	/// The return value for this node.
	/// The type of the return value is determined by the node type.
	/// To be changed in the Provoke() method.
	/// </summary>
	static void* m_pvReturn;
	/// <summary>
	/// True if that node replaces the provoking node's return value;
	/// </summary>
	bool m_bReturn;
	/// <summary>
	/// Node behavior for the next provoking circle.
	///	Set back to >DefaultBehavior< after next call.
	/// </summary>
	AQU_NextNodeCall m_eNextNodeCall;
	/// <summary>
	/// The provoker/invoker setting of this node.
	/// </summary>
	AQU_NodeProvokingType m_eNodeProvokingType;
	/// <summary>
	/// Vector to all node decommanders.
	/// </summary>
	std::vector<NOD_Decommander*> m_paDecommanders;

protected:
	/// <summary>
	/// Node invoker.
	/// </summary>
	NOD_Invoker m_cInvoker;
	/// <summary>
	/// The size of the node in the canvas (in full zoom pixel space).
	/// </summary>
	ImVec2 m_sSize;
	/// <summary>
	/// Current zoom factor, needed for node translation.
	/// </summary>
	float m_fZoom;
	/// <summary>
	/// Added to node width, reserved for slots.
	/// </summary>
	float m_fSlotSpace;
	/// <summary>
	/// Current local mouse cursor position.
	/// </summary>
	POINT m_vecLocalMouseCursor;
	/// <summary>
	/// The mouse cursor origin for all mouse actions (in desktop pixel space).
	/// </summary>
	POINT m_vecMouseOrigin;
	/// <summary>
	/// The behavior of the node.
	/// </summary>
	AQU_NodeBehavior m_NodeBehavior;
	/// <summary>
	/// The last chosen commander index.
	/// </summary>
	int m_nCommanderIndex;
	/// <summary>
	/// The connection origin to draw a new connection.
	/// </summary>
	POINT m_vecConnectionOrigin;
	/// <summary>
	/// True if the last connected invoker is to be deleted within the next Provoke() call.
	/// This must be done within the Provoke() call since this is the only safe way.
	/// Otherwise the provoking circle could end up nowhere causing a crash.
	/// </summary>
	bool m_bPopBackConnectedInvokers;
	/// <summary>
	/// Node index to be verified.
	///-1 if no node to be verified.
	/// </summary>
	int m_nNodeIndexToVerify;
	/// <summary>
	/// True after that node is drawn the first time.
	/// </summary>
	bool m_bFirstDraw;
};

#endif