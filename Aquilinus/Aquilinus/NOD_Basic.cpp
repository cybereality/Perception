/********************************************************************
Vireio Perception : Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Aquilinus : Vireio Perception 3D Modification Studio 
Copyright � 2014 Denis Reischl

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
	m_vecPos.x = nX;
	m_vecPos.y = nY;

	m_fXPos = (float)nX;
	m_fYPos = (float)nY;

	m_vecSize.cx = nWidth;
	m_vecSize.cy = nHeight;

	m_szTitle = L"Basic Node";
	m_NodeBehavior = AQU_NodeBehavior::Inactive;
	m_eNodeProvokingType = AQU_NodeProvokingType::None;
	m_eNextNodeCall = AQU_NextNodeCall::DefaultBehavior;

	m_pvReturn = NULL;
	m_bReturn = false;

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
* Moves the node to the desired position.
***/
HRESULT NOD_Basic::Translate(LONG nX, LONG nY, float fZoom)
{
	// TODO !! MAX X/Y RANGE

	m_fXPos += nX / fZoom;
	m_fYPos += nY / fZoom;

	if (m_fXPos < 0.0f) m_fXPos = 0.0f;
	if (m_fYPos < 0.0f) m_fYPos = 0.0f;

	m_vecPos.x = (LONG)m_fXPos;
	m_vecPos.y = (LONG)m_fYPos;

	return S_OK;
}

/**
* Draws the node.
* The basic node draws the border and the connectors.
* @param pDirectDraw The aquilinus direct draw interface.
* @param vcOrigin The origin vector for the drawing call, in pixel space.
***/
HRESULT NOD_Basic::Draw(AQU_Drawer* pDirectDraw, POINT vcOrigin, float fZoom)
{
	// clear node background
	pDirectDraw->DrawHLine(m_vecPos, vcOrigin, m_vecSize.cx, m_vecSize.cy, AquilinusColor::Color1, fZoom);

	// draw the border
	POINT pt = m_vecPos;
	pDirectDraw->DrawHLine(pt, vcOrigin, m_vecSize.cx, 4, AquilinusColor::Color3, fZoom);

	pt = m_vecPos;
	pt.y += m_vecSize.cy - 4;
	pDirectDraw->DrawHLine(pt, vcOrigin, m_vecSize.cx, 4, AquilinusColor::Color3, fZoom);

	pt = m_vecPos;
	pDirectDraw->DrawVLine(pt, vcOrigin, m_vecSize.cy, 4, AquilinusColor::Color3, fZoom);

	pt = m_vecPos;
	pt.x += m_vecSize.cx - 4;
	pDirectDraw->DrawVLine(pt, vcOrigin, m_vecSize.cy, 4, AquilinusColor::Color3, fZoom);

	// draw the text
	DWORD dwWidth = pDirectDraw->GetTextWidth(m_szTitle, AQUILINUS_TINY_TEXT_SIZE, fZoom);
	pt = m_vecPos;
	pt.y += 16;
	pt.x += (m_vecSize.cx >> 1) - ((LONG)dwWidth >> 1);
	pDirectDraw->RenderText(m_szTitle, pt.x, pt.y, vcOrigin, AquilinusColor::Color2, AquilinusColor::Color1, AQUILINUS_TINY_TEXT_SIZE, fZoom);

	// draw the decommanders
	pt.y += 24;
	for (std::vector<NOD_Decommander*>::size_type i = 0; i != m_paDecommanders.size(); i++)
	{
		// draw the joint
		pt.x = m_vecPos.x;
		pDirectDraw->DrawHLine(pt, vcOrigin, 8, 4, AquilinusColor::Color4, fZoom);

		// set the decommander y pos
		m_paDecommanders[i]->m_lNodeYPos = pt.y - m_vecPos.y;

		// get text width, set x pos
		dwWidth = pDirectDraw->GetTextWidth(m_paDecommanders[i]->m_szTitle, AQUILINUS_TINY_TEXT_SIZE, fZoom);
		pt.y -= 2;
		pt.x += 12;

		// verify node width
		if (m_vecSize.cx < ((LONG)dwWidth + 24)) m_vecSize.cx = (LONG)dwWidth + 24;

		pDirectDraw->RenderText(m_paDecommanders[i]->m_szTitle, pt.x, pt.y, vcOrigin, AquilinusColor::Color3, AquilinusColor::Color1, AQUILINUS_TINY_TEXT_SIZE, fZoom);

		pt.y += 16;
	}

	// draw the commanders
	for (std::vector<NOD_Commander*>::size_type i = 0; i != m_paCommanders.size(); i++)
	{
		// draw the joint
		pt.x = m_vecPos.x + m_vecSize.cx - 8;
		pDirectDraw->DrawHLine(pt, vcOrigin, 8, 4, AquilinusColor::Color4, fZoom);

		// set the decommander y pos
		m_paCommanders[i]->m_lNodeYPos = pt.y - m_vecPos.y;

		// get text width, set x pos
		dwWidth = pDirectDraw->GetTextWidth(m_paCommanders[i]->m_szTitle, AQUILINUS_TINY_TEXT_SIZE, fZoom);
		pt.y -= 2;
		pt.x = m_vecPos.x + m_vecSize.cx - (LONG)dwWidth - 16;

		// verify node width
		if (m_vecSize.cx < ((LONG)dwWidth + 24)) m_vecSize.cx = (LONG)dwWidth + 24;

		pDirectDraw->RenderText(m_paCommanders[i]->m_szTitle, pt.x, pt.y, vcOrigin, AquilinusColor::Color3, AquilinusColor::Color1, AQUILINUS_TINY_TEXT_SIZE, fZoom);

		pt.y += 16;
	}

	if ((m_eNodeProvokingType == AQU_NodeProvokingType::OnlyInvoker) || (m_eNodeProvokingType == AQU_NodeProvokingType::Both))
	{
		// draw the invoker joint
		pt.x = m_vecPos.x;
		pDirectDraw->DrawHLine(pt, vcOrigin, 8, 4, AquilinusColor::Color2, fZoom);
	}

	if ((m_eNodeProvokingType == AQU_NodeProvokingType::OnlyProvoker) || (m_eNodeProvokingType == AQU_NodeProvokingType::Both))
	{
		// draw the provoker joint
		pt.x = m_vecPos.x + m_vecSize.cx - 8;
		pDirectDraw->DrawHLine(pt, vcOrigin, 8, 4, AquilinusColor::Color2, fZoom);
	}

	if (m_eNodeProvokingType != AQU_NodeProvokingType::None)
	{
		// set provoker/invoker y pos
		m_cInvoker.m_lNodeYPos = pt.y - m_vecPos.y;
		m_cProvoker.m_lNodeYPos = pt.y - m_vecPos.y;

		pt.y += 16;
	}

	// verify node height
	if (m_vecSize.cy < (pt.y - m_vecPos.y + 16)) m_vecSize.cy = pt.y - m_vecPos.y + 16;

	return S_OK;
}

/**
* Draw all connections from the Commanders of this node.
* @param pDirectDraw The aquilinus direct draw interface.
* @param vcOrigin The origin vector for the drawing call, in pixel space.
* @param fZoom The current workspace zoom factor.
* @param ppaNodes The vector of ALL nodes (pointer).
***/
HRESULT NOD_Basic::DrawConnections(AQU_Drawer* pDirectDraw, POINT vcOrigin, float fZoom, std::vector<NOD_Basic*>* ppaNodes, AQU_ConnectionDrawingStyles eDataConnectionStyle, AQU_ConnectionDrawingStyles eStreamConnectionStyle)
{
	// draw commander-decommander connections
	for (std::vector<NOD_Commander*>::size_type i = 0; i != m_paCommanders.size(); i++)
	{
		for (std::vector<NOD_Decommander*>::size_type j = 0; j != m_paCommanders[i]->m_paDecommanders.size(); j++)
		{
			// get connection source
			POINT vecS = m_vecPos;
			vecS.x += m_vecSize.cx;
			vecS.y += m_paCommanders[i]->m_lNodeYPos;

			// get destination node index
			LONG lDestNodeIndex = m_paCommanders[i]->m_paDecommanders[j]->m_lNodeIndex;

			// get connection destination
			POINT vecD = (*ppaNodes)[lDestNodeIndex]->m_vecPos;
			vecD.y += m_paCommanders[i]->m_paDecommanders[j]->m_lNodeYPos;

			// adjust y to joint middle
			vecS.y += 2;
			vecD.y += 2;

			// and draw
			switch (eDataConnectionStyle)
			{
				case Line:
					pDirectDraw->DrawLine(vecS, vecD, vcOrigin, 4, AquilinusColor::Color4, fZoom);
					break;
				case Sling:
					//pDirectDraw->DrawEdge(vecS, vecD, vcOrigin, 4, AquilinusColor::Color4, true, fZoom);
					break;
				case Pipeline:
					pDirectDraw->DrawPipeline(vecS, vecD, vcOrigin, 4, AquilinusColor::Color4, fZoom);
					break;
				case DottedLine:
					pDirectDraw->DrawLine(vecS, vecD, vcOrigin, 4, AquilinusColor::Color4, fZoom);
					break;
				case DottedSling:
					//pDirectDraw->DrawEdge(vecS, vecD, vcOrigin, 4, AquilinusColor::Color4, true, fZoom);
					break;
				case DottedPipeline:
					pDirectDraw->DrawPipeline(vecS, vecD, vcOrigin, 4, AquilinusColor::Color4, fZoom);
					break;
				default:
					break;
			}

		}
	}

	// draw provoker-invoker connections
	for (std::vector<NOD_Invoker*>::size_type j = 0; j != m_cProvoker.m_paInvokers.size(); j++)
	{
		// get connection source
		POINT vecS = m_vecPos;
		vecS.x += m_vecSize.cx;
		vecS.y += m_cProvoker.m_lNodeYPos;

		// get destination node index
		LONG lDestNodeIndex = m_cProvoker.m_paInvokers[j]->m_lNodeIndex;

		// get connection destination
		POINT vecD = (*ppaNodes)[lDestNodeIndex]->m_vecPos;
		vecD.y += m_cProvoker.m_paInvokers[j]->m_lNodeYPos;

		// and draw, first pipeline will be drawn in other color
		AquilinusColor eColor;
		if (j == 0)
			eColor = AquilinusColor::Color2;
		else
			eColor = AquilinusColor::Color3;

		// adjust y to joint middle
		vecS.y += 2;
		vecD.y += 2;

		// and draw
		switch (eStreamConnectionStyle)
		{
			case Line:
				pDirectDraw->DrawLine(vecS, vecD, vcOrigin, 4, eColor, fZoom);
				break;
			case Sling:
				//pDirectDraw->DrawEdge(vecS, vecD, vcOrigin, 4, eColor, true, fZoom);
				break;
			case Pipeline:
				pDirectDraw->DrawPipeline(vecS, vecD, vcOrigin, 4, eColor, fZoom);
				break;
			case DottedLine:
				pDirectDraw->DrawLine(vecS, vecD, vcOrigin, 4, eColor, fZoom);
				break;
			case DottedSling:
				//pDirectDraw->DrawEdge(vecS, vecD, vcOrigin, 4, eColor, true, fZoom);
				break;
			case DottedPipeline:
				pDirectDraw->DrawPipeline(vecS, vecD, vcOrigin, 4, eColor, fZoom);
				break;
			default:
				break;
		}
	}

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
	switch (msg)
	{
		// left mouse button down ?
		case WM_LBUTTONDOWN:
			// node joint ? set connect behavior;
			// first : commanders
			if (m_vecLocalMouseCursor.x > m_vecSize.cx - 8)
			{
				// mouse cursor is at the right border, so loop through commanders and provokers
				for (std::vector<NOD_Commander*>::size_type i = 0; i != m_paCommanders.size(); i++)
				{
					if ((m_vecLocalMouseCursor.y >= m_paCommanders[i]->m_lNodeYPos) &&
						(m_vecLocalMouseCursor.y <= (m_paCommanders[i]->m_lNodeYPos + 4)))
					{
						// set node behavior, commander index and the origin of the connection
						m_NodeBehavior = AQU_NodeBehavior::ConnectCommander;
						m_nCommanderIndex = (int)i;
						m_vecConnectionOrigin.x = m_vecPos.x + m_vecSize.cx;
						m_vecConnectionOrigin.y = m_vecPos.y + m_paCommanders[i]->m_lNodeYPos;
					}
				}

				// provokers
				if ((m_vecLocalMouseCursor.y >= m_cProvoker.m_lNodeYPos) &&
					(m_vecLocalMouseCursor.y <= (m_cProvoker.m_lNodeYPos + 4)) &&
					((m_eNodeProvokingType == AQU_NodeProvokingType::OnlyProvoker) || (m_eNodeProvokingType == AQU_NodeProvokingType::Both)))
				{
					// set node behavior and the origin of the connection
					m_NodeBehavior = AQU_NodeBehavior::ConnectProvoker;
					m_vecConnectionOrigin.x = m_vecPos.x + m_vecSize.cx;
					m_vecConnectionOrigin.y = m_vecPos.y + m_cProvoker.m_lNodeYPos;
				}
			}
			else if ((m_NodeBehavior != AQU_NodeBehavior::ConnectCommander) &&
				(m_NodeBehavior != AQU_NodeBehavior::ConnectProvoker))
			{
				if (m_vecLocalMouseCursor.y < (LONG)m_dwHeaderSize)
				{
					// set mouse cursor origin and >translate< behavior
					m_vecMouseOrigin.x = GET_X_LPARAM(lParam);
					m_vecMouseOrigin.y = GET_Y_LPARAM(lParam);
					m_NodeBehavior = AQU_NodeBehavior::Translate;
				}
				else
					m_NodeBehavior = AQU_NodeBehavior::WindowsEvent;
			}
			break;
			// left mouse button up ?
		case WM_LBUTTONUP:
			switch (m_NodeBehavior)
			{
				case AQU_NodeBehavior::ConnectCommander:
				case AQU_NodeBehavior::ConnectProvoker:
					// set this node inactive, connection is done in the working area class before
					m_NodeBehavior = AQU_NodeBehavior::Inactive;
					break;
				case AQU_NodeBehavior::Translate:
				case AQU_NodeBehavior::WindowsEvent:
					// set this node inactive
					m_NodeBehavior = AQU_NodeBehavior::Inactive;
					break;
			}
			break;
			// mouse move ?
		case WM_MOUSEMOVE:
			switch (m_NodeBehavior)
			{
				case AQU_NodeBehavior::Translate:
					// translate to the new position and then set new mouse cursor origin
					Translate(GET_X_LPARAM(lParam) - m_vecMouseOrigin.x, GET_Y_LPARAM(lParam) - m_vecMouseOrigin.y, m_fZoom);
					m_vecMouseOrigin.x = GET_X_LPARAM(lParam);
					m_vecMouseOrigin.y = GET_Y_LPARAM(lParam);
					break;
			}
			break;
		case WM_RBUTTONDOWN:
			// delete connections by right click on the commander/provoker
			if (m_vecLocalMouseCursor.x > m_vecSize.cx - 8)
			{
				// mouse cursor is at the right border, so loop through commanders and provokers
				for (std::vector<NOD_Commander*>::size_type i = 0; i != m_paCommanders.size(); i++)
				{
					if ((m_vecLocalMouseCursor.y >= m_paCommanders[i]->m_lNodeYPos) &&
						(m_vecLocalMouseCursor.y <= (m_paCommanders[i]->m_lNodeYPos + 4)))
					{
						// is there any connection present ?
						if (m_paCommanders[i]->m_paDecommanders.size() > 0)
						{
							// delete commander pointer in the decommander struct of the last connected node
							auto it = std::find(m_paCommanders[i]->m_paDecommanders.back()->m_paCommanders.begin(), m_paCommanders[i]->m_paDecommanders.back()->m_paCommanders.end(), m_paCommanders[i]);
							if (it != m_paCommanders[i]->m_paDecommanders.back()->m_paCommanders.end())
							{
								// set node index of the connected decommanders node to be verified
								m_nNodeIndexToVerify = m_paCommanders[i]->m_paDecommanders.back()->m_lNodeIndex;

								// and erase the connected commander of this node
								m_paCommanders[i]->m_paDecommanders.back()->m_paCommanders.erase(it);
							}

							// delete last element 
							m_paCommanders[i]->m_paDecommanders.pop_back();
						}
					}
				}

				// provokers
				if ((m_vecLocalMouseCursor.y >= m_cProvoker.m_lNodeYPos) &&
					(m_vecLocalMouseCursor.y <= (m_cProvoker.m_lNodeYPos + 4)))
				{
					// is there any connection present ?
					if (m_cProvoker.m_paInvokers.size() > 0)
					{
						// delete provoker pointer in the invoker struct of the last connected node
						auto it = std::find(m_cProvoker.m_paInvokers.back()->m_paProvokers.begin(), m_cProvoker.m_paInvokers.back()->m_paProvokers.end(), &m_cProvoker);
						if (it != m_cProvoker.m_paInvokers.back()->m_paProvokers.end())
							m_cProvoker.m_paInvokers.back()->m_paProvokers.erase(it);

						// create an event on that since that can only be done within the Provoke() method in a safe way
						m_bPopBackConnectedInvokers = true;
					}

				}
			}
			break;
	}

	return m_NodeBehavior;
}

/**
* Tests wether the cursor is within the nodes border.
* @param vecCursor The position of the mouse cursor.
* @param vcOrigin The origin of the render surface, in pixel space.
* @param fZoom The zoom factor, will be saved in the node class.
* @returns True if the cursor is within the nodes border.
***/
bool NOD_Basic::IsLocal(POINT vecCursor, POINT vcOrigin, float fZoom)
{
	// set zoom factor
	m_fZoom = fZoom;

	LONG nXDif = (vecCursor.x - vcOrigin.x) - (LONG)(m_fXPos * fZoom);
	LONG nYDif = (vecCursor.y - vcOrigin.y) - (LONG)(m_fYPos * fZoom);

	if ((nXDif >= 0) &&
		(nXDif <= (LONG)((float)(m_vecSize.cx * fZoom))) &&
		(nYDif >= 0) &&
		(nYDif <= (LONG)((float)(m_vecSize.cy * fZoom))))
	{
		m_vecLocalMouseCursor.x = (LONG)((float)nXDif / fZoom);
		m_vecLocalMouseCursor.y = (LONG)((float)nYDif / fZoom);

		return true;
	}

	return false;
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
* This method returns the origin of a new connection.
* Used only when node is in connection behavior.
* @returns The origin of the new connection.
***/
POINT NOD_Basic::GetConnectionOrigin()
{
	return m_vecConnectionOrigin;
}

/**
* Connects the decommander of this node with the commander of the provided node, if cursor hits its position.
***/
void NOD_Basic::ConnectCommander(NOD_Basic* pNode, LONG nThisNodeIndex)
{
	if ((m_vecLocalMouseCursor.x < 8) && (pNode))
	{
		// mouse cursor is at the left border, so loop through decommanders 
		for (std::vector<NOD_Decommander*>::size_type i = 0; i != m_paDecommanders.size(); i++)
		{
			// is the mouse cursor at a decommander plug and does the decommander share the same connection rule ? (VOID decommander connects to everything)
			if ((m_vecLocalMouseCursor.y >= m_paDecommanders[i]->m_lNodeYPos) &&
				(m_vecLocalMouseCursor.y <= (m_paDecommanders[i]->m_lNodeYPos + 4)) &&
				((pNode->m_paCommanders[pNode->m_nCommanderIndex]->m_ePlugtype == m_paDecommanders[i]->m_ePlugtype) || (m_paDecommanders[i]->m_ePlugtype == NOD_Plugtype::AQU_VOID)))
			{
				// set this node index
				m_paDecommanders[i]->m_lNodeIndex = nThisNodeIndex;

				// connect decommander of this node
				m_paDecommanders[i]->m_paCommanders.push_back(pNode->m_paCommanders[pNode->m_nCommanderIndex]);

				// connect commander of the connection node
				pNode->m_paCommanders[pNode->m_nCommanderIndex]->m_paDecommanders.push_back(m_paDecommanders[i]);

				// set connection node data if it has no provoker !
				m_paDecommanders[i]->m_pInput = pNode->m_paCommanders[pNode->m_nCommanderIndex]->m_pOutput;

			}
		}
	}
}

/**
* Connects the commander of this node with the decommander of the provided node.
* For loading operations.
***/
void NOD_Basic::ConnectDecommander(NOD_Basic* pNode, LONG nDestNodeIndex, DWORD dwCommanderIndex, DWORD dwDecommanderIndex)
{
	// does the decommander share the same connection rule ? (VOID decommander connects to everything)
	if ((m_paCommanders[dwCommanderIndex]->m_ePlugtype == pNode->m_paDecommanders[dwDecommanderIndex]->m_ePlugtype) || (pNode->m_paDecommanders[dwDecommanderIndex]->m_ePlugtype == NOD_Plugtype::AQU_VOID))
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
* Connects the invoker of this node with the provoker of the provided node, if cursor hits its position and the provokers method call is supported.
***/
void NOD_Basic::ConnectProvoker(NOD_Basic* pNode, LONG nThisNodeIndex)
{
	if (m_vecLocalMouseCursor.x < 8)
	{
		// mouse cursor is at the left border, verify invoker
		if ((m_vecLocalMouseCursor.y >= m_cInvoker.m_lNodeYPos) &&
			(m_vecLocalMouseCursor.y <= (m_cInvoker.m_lNodeYPos + 4)) &&
			((m_eNodeProvokingType == AQU_NodeProvokingType::Both) || (m_eNodeProvokingType == AQU_NodeProvokingType::OnlyInvoker)) &&
			(SupportsD3DMethod(pNode->m_cProvoker.m_eD3D, pNode->m_cProvoker.m_eD3DInterface, pNode->m_cProvoker.m_eD3DMethod)))
		{
			// set this node index
			m_cInvoker.m_lNodeIndex = nThisNodeIndex;

			// connect provoker of the connection node
			m_cInvoker.m_paProvokers.push_back(&pNode->m_cProvoker);
			if (m_cInvoker.m_paProvokers.size() == 1)
			{
				// set node calling method if this is the first connected provoker
				m_cProvoker.m_eD3D = pNode->m_cProvoker.m_eD3D;
				m_cProvoker.m_eD3DInterface = pNode->m_cProvoker.m_eD3DInterface;
				m_cProvoker.m_eD3DMethod = pNode->m_cProvoker.m_eD3DMethod;
			}

			// connect invoker of this node
			pNode->m_cProvoker.m_paInvokers.push_back(&m_cInvoker);
		}
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
		if ((*ppaNodes)[m_cProvoker.m_paInvokers[i]->m_lNodeIndex]->m_eNextNodeCall)
		{
			this->m_eNextNodeCall = (*ppaNodes)[m_cProvoker.m_paInvokers[i]->m_lNodeIndex]->m_eNextNodeCall;
			(*ppaNodes)[m_cProvoker.m_paInvokers[i]->m_lNodeIndex]->m_eNextNodeCall = AQU_NextNodeCall::DefaultBehavior;
		}
	}

	return m_pvReturn;
}

/**
* Provides the current position of the node.
***/
POINT NOD_Basic::GetNodePosition()
{
	return m_vecPos;
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

