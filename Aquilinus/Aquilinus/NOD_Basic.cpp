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
#include"NOD_Basic.h"

/**
* Static return data value.
***/
void* NOD_Basic::m_pvReturn;

/**
* Constructor.
* @param nX X Position of the node (in full zoom pixel space).
* @param nY Y Position of the node (in full zoom pixel space).
* @param nWidth Width of the node (in full zoom pixel space).
* @param nHeight Height of the node (in full zoom pixel space).
***/
NOD_Basic::NOD_Basic(LONG nX, LONG nY, LONG nWidth, LONG nHeight)
{
	m_sPos.x = (float)nX;
	m_sPos.y = (float)nY;

	m_sSize.x = (float)nWidth;
	m_sSize.y = (float)nHeight;

	m_szTitle = L"Basic Node";
	m_NodeBehavior = AQU_NodeBehavior::Inactive;
	m_eNodeProvokingType = AQU_NodeProvokingType::None;
	m_eNextNodeCall = AQU_NextNodeCall::DefaultBehavior;

	m_pvReturn = NULL;
	m_bReturn = false;
	m_bFirstDraw = false;
	m_acTitleA = std::string();
	m_bActive = false;

	// slot space
	m_fSlotSpace = 200.f;

	m_fZoom = 1.0f;
	m_vecLocalMouseCursor.x = 0;
	m_vecLocalMouseCursor.y = 0;
	m_nCommanderIndex = 0;
	m_vecConnectionOrigin.x = 0;
	m_vecConnectionOrigin.y = 0;
	m_dwHeaderSize = 40;

	m_cProvoker.m_eD3D = -1;
	m_cProvoker.m_eD3DInterface = -1;
	m_cProvoker.m_eD3DMethod = -1;
	m_cProvoker.m_lNodeYPos = 0;
	m_cProvoker.m_paInvokers.clear();

	m_cInvoker.m_eD3D = -1;
	m_cInvoker.m_eD3DInterface = -1;
	m_cInvoker.m_eD3DMethod = -1;
	m_cInvoker.m_lNodeYPos = 0;
	m_cInvoker.m_paProvokers.clear();

	m_bPopBackConnectedInvokers = false;

	m_nNodeIndexToVerify = -1;
}

/**
* Destructor
***/
NOD_Basic::~NOD_Basic()
{
	for (std::vector<NOD_Commander*>::size_type i = 0; i != m_paCommanders.size(); i++)
		if (m_paCommanders[i])
			delete m_paCommanders[i];
	for (std::vector<NOD_Decommander*>::size_type i = 0; i != m_paDecommanders.size(); i++)
		if (m_paDecommanders[i])
			delete m_paDecommanders[i];
}

/**
* Draws (updates) the node.
* The basic node draws the border and the connectors.
* Do NOT use ImGui::Separator() here, graphical glitch.
* @param vcOrigin The origin vector for the drawing call, in pixel space.
***/
HRESULT NOD_Basic::Update()
{
	// convert title to std::string
	if (m_acTitleA.length() == 0)
		for (size_t i = 0; i < wcslen(m_szTitle); i++) m_acTitleA += (char)m_szTitle[i];

	// output title + separator
	ImGui::Text(m_acTitleA.c_str());

	return S_OK;
}

/**
* Get the commander connection indices.
* @param dwIndex The index of the node decommander.
* @returns All node indices (both node and node-decommander-index) of the node decommanders that are connected to the specified node commander.
***/
LONG* NOD_Basic::GetCommanderConnectionIndices(std::vector<NOD_Basic*>* ppaNodes, DWORD dwIndex)
{
	// return nullpointer if no indices present
	if (dwIndex >= (DWORD)m_paCommanders.size()) return nullptr;
	if (m_paCommanders[dwIndex]->m_paDecommanders.size() <= 0) return nullptr;

	// create return array
	static LONG* ret = new LONG[m_paCommanders[dwIndex]->m_paDecommanders.size() * 2];

	// loop through decommanders, return indices
	for (int i = 0; i < (int)m_paCommanders[dwIndex]->m_paDecommanders.size(); i++)
	{
		// write node index
		ret[i * 2] = m_paCommanders[dwIndex]->m_paDecommanders[i]->m_lNodeIndex;

		// write decommander index
		std::vector<NOD_Decommander*>::iterator itDecommanderIndex = std::find((*ppaNodes)[m_paCommanders[dwIndex]->m_paDecommanders[i]->m_lNodeIndex]->m_paDecommanders.begin(),
			(*ppaNodes)[m_paCommanders[dwIndex]->m_paDecommanders[i]->m_lNodeIndex]->m_paDecommanders.end(),
			m_paCommanders[dwIndex]->m_paDecommanders[i]);
		if (itDecommanderIndex == (*ppaNodes)[m_paCommanders[dwIndex]->m_paDecommanders[i]->m_lNodeIndex]->m_paDecommanders.end())
		{
			delete ret;
			return nullptr;
		}
		ret[i * 2 + 1] = (LONG)(itDecommanderIndex - (*ppaNodes)[m_paCommanders[dwIndex]->m_paDecommanders[i]->m_lNodeIndex]->m_paDecommanders.begin());
	}

	// and return
	return ret;
}

/**
* Get the provoker connection indices.
* @returns All node indices of the node invokers that are connected to this node provoker.
***/
LONG* NOD_Basic::GetProvokerConnectionIndices()
{
	// return nullpointer if no indices present
	if (m_cProvoker.m_paInvokers.size() <= 0) return nullptr;

	// create return array
	static LONG* ret = new LONG[m_cProvoker.m_paInvokers.size()];

	// loop through decommanders, return indices
	for (int i = 0; i < (int)m_cProvoker.m_paInvokers.size(); i++)
		ret[i] = m_cProvoker.m_paInvokers[i]->m_lNodeIndex;

	// and return
	return ret;

}

/**
* Get the number of all connected decommanders.
* @param dwIndex The index of the commander.
* @returns The number of all connected decommanders connected to the provided commander.
***/
DWORD NOD_Basic::GetCommanderConnectionsNumber(DWORD dwIndex)
{
	return (DWORD)m_paCommanders[dwIndex]->m_paDecommanders.size();;
}

/**
* Get the number of all connected invokers.
* @returns The number of all connected invokders connected to this node provoker.
***/
DWORD NOD_Basic::GetProvokerConnectionsNumber()
{
	return (DWORD)m_cProvoker.m_paInvokers.size();
}

/**
* Get the number of connected provokers.
***/
DWORD NOD_Basic::GetInvokerConnectionsNumber()
{
	return (DWORD)m_cInvoker.m_paProvokers.size();
}

/**
* This node gets a new index.
***/
void NOD_Basic::SetNewIndex(DWORD unIndex)
{
	// set new decommander indices
	for (UINT unJ = 0; unJ < (UINT)m_paDecommanders.size(); unJ++)
		m_paDecommanders[unJ]->m_lNodeIndex = (LONG)unIndex;

	// set new indices for provoker, invoker
	m_cInvoker.m_lNodeIndex = (LONG)unIndex;
	m_cProvoker.m_lNodeIndex = (LONG)unIndex;
}

/**
* Window Message event function.
* Called whenever the mouse pointer is within the nodes working range.
***/
AQU_NodeBehavior NOD_Basic::WindowsEvent(UINT msg, WPARAM wParam, LPARAM lParam)
{

	return m_NodeBehavior;
}

/**
* True if this Node supports the requested D3D method call.
* @param Direct3D version to be used by the calling provoker. Equates AQU_Direct3DVersion.
* @param Direct3D interface index. Equates ITA_D3D9Interfaces, ITA_D3D10Interfaces, .... depending on the D3D version defined in eD3D.
* @param Direct3D method index. Equates VMT_*whatever* virtual methods table.... depending on the D3D version defined in eD3D and the interface defined in eD3DInterface.
***/
bool NOD_Basic::SupportsD3DMethod(int eD3D, int eD3DInterface, int eD3DMethod)
{
	// basic node supports any device, interface and its methods, so return >true<
	return true;
}

/**
* Node input slot/link/connection. TODO !! ZOOM
***/
void NOD_Basic::InputSlot(const char* title, int kind) 
{
	// set cursor
	const auto& style = ImGui::GetStyle();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - style.ItemSpacing.x); //  * zoom);

	// get text size, connection circle
	auto* draw_lists = ImGui::GetWindowDrawList();
	ImVec2 title_size = ImGui::CalcTextSize(title);
	ImRect circle_rect{
			ImGui::GetCursorScreenPos(),
			ImGui::GetCursorScreenPos() + ImVec2{10.f, 10.f}
	};

	// Vertical-align circle in the middle of the line.
	float circle_offset_y = title_size.y / 2.f - 5.f;
	circle_rect.Min.y += circle_offset_y;
	circle_rect.Max.y += circle_offset_y;
	draw_lists->AddCircleFilled(circle_rect.GetCenter(), 5.f, 0xffffffff);

	// add item
	ImGui::ItemSize(circle_rect.GetSize());
	ImGui::ItemAdd(circle_rect, ImGui::GetID(title));

	// if (bDrawSlotTitles) {} // TODO !!!
	{
		ImGui::SameLine();
		ImGui::TextUnformatted(title);
	}
	/*else
	{
		ImGui::SameLine();
		ImGui::TextUnformatted(" ");
	}*/

	ImGui::SameLine();
	ImGui::GetCurrentWindow()->DC.CursorMaxPos.x -= style.ItemSpacing.x;
}

/**
* Node output slot/link/connection.
**/
void NOD_Basic::OutputSlot(const char* title, int kind) 
{
	// set cursor
	const auto& style = ImGui::GetStyle();
	float fPos = ImGui::GetCursorPosX() + m_fSlotSpace + m_sSize.x + style.ItemSpacing.x;
	ImVec2 title_size = ImGui::CalcTextSize(title);
	ImGui::SetCursorPosX(fPos - title_size.x - style.ItemSpacing.x); //  * zoom);

	// if (bDrawSlotTitles) {} // TODO !!!
	{
		ImGui::TextUnformatted(title);
		ImGui::SameLine();
	}
	/*else
	{
		ImGui::TextUnformatted(" ");
		ImGui::SameLine();
	}*/

	ImGui::SetCursorPosX(fPos); //  * zoom);

	// get text size, connection circle
	auto* draw_lists = ImGui::GetWindowDrawList();
	ImRect circle_rect{
			ImGui::GetCursorScreenPos(),
			ImGui::GetCursorScreenPos() + ImVec2{10.f, 10.f}
	};

	// Vertical-align circle in the middle of the line.
	float circle_offset_y = title_size.y / 2.f - 5.f;
	circle_rect.Min.y += circle_offset_y;
	circle_rect.Max.y += circle_offset_y;
	draw_lists->AddCircleFilled(circle_rect.GetCenter(), 5.f, 0xffffffff);

	// add item
	ImGui::ItemSize(circle_rect.GetSize());
	ImGui::ItemAdd(circle_rect, ImGui::GetID(title));

	ImGui::SameLine();
	ImGui::GetCurrentWindow()->DC.CursorMaxPos.x -= style.ItemSpacing.x;
}

/**
* This method returns the origin of a new connection.
* Used only when node is in connection behavior.
* @returns The origin of the new connection.
***/
POINT NOD_Basic::GetConnectionOrigin()
{
	return m_vecConnectionOrigin;
}

/**
* Connects the commander of this node with the decommander of the provided node.
***/
void NOD_Basic::ConnectDecommander(NOD_Basic* pNode, LONG nDestNodeIndex, std::string acCommander, std::string acDecommander)
{
	if (!pNode) return;
	NOD_Commander* psCommander = nullptr;
	NOD_Decommander* psDecommander = nullptr;

	// get commander, decommander
	for (NOD_Commander* pC : m_paCommanders) 
		if (acCommander.compare(pC->m_szTitleA) == 0) psCommander = pC;
	for (NOD_Decommander* pD : pNode->m_paDecommanders)
		if (acDecommander.compare(pD->m_szTitleA) == 0) psDecommander = pD;

	if ((psCommander) && (psDecommander))
	{
		// same plugtype ?
		if (psCommander->m_ePlugtype == psDecommander->m_ePlugtype)
		{
			// set destination node index
			psDecommander->m_lNodeIndex = nDestNodeIndex;

			// connect commander of the connection node
			psDecommander->m_paCommanders.push_back(psCommander);

			// connect commander of this node
			psCommander->m_paDecommanders.push_back(psDecommander);

			// set connection node data if it has no provoker !
			psDecommander->m_pInput = psCommander->m_pOutput;
		}
	}
}


/**
* Connects the commander of this node with the decommander of the provided node.
***/
void NOD_Basic::ConnectDecommander(NOD_Basic* pNode, LONG nDestNodeIndex, DWORD dwCommanderIndex, DWORD dwDecommanderIndex)
{
	// does the decommander share the same connection rule ? 
	if (m_paCommanders[dwCommanderIndex]->m_ePlugtype == pNode->m_paDecommanders[dwDecommanderIndex]->m_ePlugtype)
	{
		// set destination node index
		pNode->m_paDecommanders[dwDecommanderIndex]->m_lNodeIndex = nDestNodeIndex;

		// connect commander of the connection node
		pNode->m_paDecommanders[dwDecommanderIndex]->m_paCommanders.push_back(m_paCommanders[dwCommanderIndex]);

		// connect commander of this node
		m_paCommanders[dwCommanderIndex]->m_paDecommanders.push_back(pNode->m_paDecommanders[dwDecommanderIndex]);

		// set connection node data if it has no provoker !
		pNode->m_paDecommanders[dwDecommanderIndex]->m_pInput = m_paCommanders[dwCommanderIndex]->m_pOutput;
	}
}

/**
* Connects the provoker of this node with the invoker of the provided node.
* For loading operations.
***/
void NOD_Basic::ConnectInvoker(NOD_Basic* pNode, LONG nDestNodeIndex)
{
	// set destination node index
	pNode->m_cInvoker.m_lNodeIndex = nDestNodeIndex;

	// connect provoker of this node
	pNode->m_cInvoker.m_paProvokers.push_back(&m_cProvoker);

	// in case a non d3d method node is loaded first it maybe has default d3d values
	if ((pNode->m_cInvoker.m_paProvokers.size() == 1) ||
		(pNode->m_cProvoker.m_eD3DInterface == -1) ||
		(pNode->m_cProvoker.m_eD3D == -1) ||
		(pNode->m_cProvoker.m_eD3DMethod == -1))
	{
		// set node calling method if this is the first connected provoker
		pNode->m_cProvoker.m_eD3D = m_cProvoker.m_eD3D;
		pNode->m_cProvoker.m_eD3DInterface = m_cProvoker.m_eD3DInterface;
		pNode->m_cProvoker.m_eD3DMethod = m_cProvoker.m_eD3DMethod;
	}

	// connect invoker of the connection node
	m_cProvoker.m_paInvokers.push_back(&pNode->m_cInvoker);
}

/**
* Aligns the data pointer for a node decommander.
***/
void NOD_Basic::AlignData(LONG nDecommanderIndex, void* pData)
{
	m_paDecommanders[nDecommanderIndex]->m_pInput = pData;
}

/**
* The provoking method.
* The type of the return value is determined by the node type.
***/
void* NOD_Basic::Provoke(void* pcThis, int eD3D, int eD3DInterface, int eD3DMethod, std::vector<NOD_Basic*>* ppaNodes)
{
	// update the commanders->decommanders data pointers only if this node has no invoker
	if (m_eNodeProvokingType == AQU_NodeProvokingType::OnlyProvoker)
	{
		// loop through connected decommanders, command.
		for (std::vector<NOD_Commander*>::size_type i = 0; i != m_paCommanders.size(); i++)
		{
			for (std::vector<NOD_Decommander*>::size_type j = 0; j != m_paCommanders[i]->m_paDecommanders.size(); j++)
			{
				// set data pointer
				(*ppaNodes)[m_paCommanders[i]->m_paDecommanders[j]->m_lNodeIndex]->AlignData(m_paCommanders[i]->m_paDecommanders[j]->m_lDecommanderIndex, m_paCommanders[i]->m_pOutput);
			}
		}
	}

	// node to be verified ?
	if (m_nNodeIndexToVerify >= 0)
	{
		if (m_nNodeIndexToVerify < (int)(*ppaNodes).size())
			(*ppaNodes)[m_nNodeIndexToVerify]->VerifyConnections(ppaNodes);
		m_nNodeIndexToVerify = -1;
	}

	// is there an invoker to be deleted ? this can ONLY be done HERE in a safe way
	if (m_bPopBackConnectedInvokers)
	{
		// delete last element, do not try again
		if (m_cProvoker.m_paInvokers.size() > 0)
			m_cProvoker.m_paInvokers.pop_back();
		m_bPopBackConnectedInvokers = false;
	}

	// loop through connected invokers, provoke. (first set back node behavior for this call)
	for (std::vector<NOD_Invoker*>::size_type i = 0; i != m_cProvoker.m_paInvokers.size(); i++)
	{
		// only the first connected nodes result will be returned if this node replaces the provoking nodes return value
		if ((i = 0) && ((*ppaNodes)[m_cProvoker.m_paInvokers[i]->m_lNodeIndex]->m_bReturn))
			m_pvReturn = (*ppaNodes)[m_cProvoker.m_paInvokers[i]->m_lNodeIndex]->Provoke(pcThis, eD3D, eD3DInterface, eD3DMethod, ppaNodes);
		else
			(*ppaNodes)[m_cProvoker.m_paInvokers[i]->m_lNodeIndex]->Provoke(pcThis, eD3D, eD3DInterface, eD3DMethod, ppaNodes);

		// is there a special setting for the next provoking circle ?
		if ((*ppaNodes)[m_cProvoker.m_paInvokers[i]->m_lNodeIndex]->m_eNextNodeCall != AQU_NextNodeCall::DefaultBehavior)
		{
			this->m_eNextNodeCall = (*ppaNodes)[m_cProvoker.m_paInvokers[i]->m_lNodeIndex]->m_eNextNodeCall;
			(*ppaNodes)[m_cProvoker.m_paInvokers[i]->m_lNodeIndex]->m_eNextNodeCall = AQU_NextNodeCall::DefaultBehavior;
		}
	}

	return m_pvReturn;
}

/*
* Returns the size of the node header text, in case the node has no image header.
*/
ImVec2 NOD_Basic::GetNodeHeaderTextSize()
{
	// convert title to std::string
	if (m_acTitleA.length() == 0)
		for (size_t i = 0; i < wcslen(m_szTitle); i++) m_acTitleA += (char)m_szTitle[i];

	return ImGui::CalcTextSize(m_acTitleA.c_str());
}


/**
* Provides the size of the save data block.
* @returns The size of the to be saved data block, in bytes.
***/
DWORD NOD_Basic::GetSaveDataSize()
{
	return 0;
}

/**
* Provides the save data block:
* @param pData The data save block, to be saved by the calling method.
* @param dwSizeOfData The size of the to be saved data block, in bytes.
***/
char* NOD_Basic::GetSaveData(UINT* pdwSizeOfData)
{
	*pdwSizeOfData = 0;

	return nullptr;
}

/**
* Verifies the connections of the node.
***/
void NOD_Basic::VerifyConnections(std::vector<NOD_Basic*>* ppaNodes)
{
	// node to be verified ?
	if (m_nNodeIndexToVerify >= 0)
	{
		if (m_nNodeIndexToVerify < (int)(*ppaNodes).size())
			(*ppaNodes)[m_nNodeIndexToVerify]->VerifyConnections(ppaNodes);
		m_nNodeIndexToVerify = -1;
	}

	// loop through decommanders, set input pointer
	for (std::vector<NOD_Decommander*>::size_type i = 0; i != m_paDecommanders.size(); i++)
	{
		// verify pointers
		if (m_paDecommanders[i]->m_paCommanders.size() == 0)
			m_paDecommanders[i]->m_pInput = nullptr;
		else
			m_paDecommanders[i]->m_pInput = m_paDecommanders[i]->m_paCommanders.back()->m_pOutput;
	}
}

/**
* Convert all strings (both commanders and decommanders).
**/
void NOD_Basic::ConvertStrings()
{
	for (std::vector<NOD_Decommander*>::size_type i = 0; i != m_paDecommanders.size(); i++)
	{
		int nLength = wcslen(m_paDecommanders[i]->m_szTitle);
		if (nLength > 32) nLength = 32;
		for (size_t j = 0; j < (size_t)nLength; j++) m_paDecommanders[i]->m_szTitleA[j] = (char)m_paDecommanders[i]->m_szTitle[j];
	}

	for (std::vector<NOD_Commander*>::size_type i = 0; i != m_paCommanders.size(); i++)
	{
		int nLength = wcslen(m_paCommanders[i]->m_szTitle);
		if (nLength > 32) nLength = 32;
		for (size_t j = 0; j < (size_t)nLength; j++) m_paCommanders[i]->m_szTitleA[j] = (char)m_paCommanders[i]->m_szTitle[j];
	}
}


