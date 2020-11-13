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
#ifndef NOD_PLUGIN
#define NOD_PLUGIN

#include"NOD_Basic.h"
#include"AQU_GlobalTypes.h"

/**
* Plugin node.
***/
class NOD_Plugin : public NOD_Basic 
{ 
public: 
	NOD_Plugin(LONG nX, LONG nY, LPWSTR szFilePath);
	~NOD_Plugin();

	/*** NOD_Basic public methods ***/
	virtual HRESULT          Draw(AQU_Drawer* pDirectDraw, POINT vcOrigin, float fZoom);
	virtual AQU_NodeBehavior WindowsEvent(UINT msg, WPARAM wParam, LPARAM lParam);
	virtual bool             SupportsD3DMethod(int eD3D, int eD3DInterface, int eD3DMethod);
	virtual void             ConnectCommander(NOD_Basic* pNode, LONG nThisNodeIndex);
	virtual void             ConnectProvoker(NOD_Basic* pNode, LONG nThisNodeIndex);
	virtual void             ConnectInvoker(NOD_Basic* pNode, LONG nDestNodeIndex);
	virtual void             AlignData(LONG nDecommanderIndex, void* pData) { m_pNodus->SetInputPointer((DWORD)nDecommanderIndex, pData); NOD_Basic::AlignData(nDecommanderIndex, pData); }
	virtual void*            Provoke(void* pcThis, int eD3D, int eD3DInterface, int eD3DMethod, std::vector<NOD_Basic*>* ppaNodes);
	virtual DWORD            GetSaveDataSize();
	virtual char*            GetSaveData(UINT* pdwSizeOfData);
	virtual void             InitNodeData(char* pData, UINT dwSizeOfData);
	virtual const char*      GetNodeType() { return typeid(this).name(); }
	virtual UINT             GetNodeTypeId() { return ELEMENTARY_NODE_PLUGIN; }
	virtual void             VerifyConnections(std::vector<NOD_Basic*>* ppaNodes);

	/*** NOD_Plugin public methods ***/
	UINT                     GetPluginNodeTypeId() { return m_pNodus->GetNodeTypeId(); }
	LPCWSTR                  GetPluginNodeFileName() { return m_szFileName; }


private:
	/**
	* The embedded nodus class.
	***/
	AQU_Nodus* m_pNodus;
	/**
	* The handle to the library.
	***/
	HMODULE m_hm;
	/**
	* The handle to the logo.
	***/
	HBITMAP m_hBitmapLogo;
	/**
	* The handle to the control.
	***/
	HBITMAP m_hBitmapControl;
	/**
	* Aquilinus drawer class backup.
	* Only used to release the bitmap IDs.
	****/
	AQU_Drawer* m_pcDrawer;
	/**
	* The logo Bitmap ID.
	***/
	UINT m_dwLogoID;
	/**
	* The control Bitmap ID.
	***/
	UINT m_dwControlID;
	/**
	* Bitmap control update counter.
	***/
	UINT m_dwUpdateCounter;
	/**
	* The control rectangle.
	***/
	RECT m_rcControl;
	/**
	* The node file name. 
	***/
	wchar_t m_szFileName[64];
};

#endif