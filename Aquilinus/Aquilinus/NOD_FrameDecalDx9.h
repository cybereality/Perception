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
#ifndef NOD_FRAMEDECALDX9
#define NOD_FRAMEDECALDX9

#include"NOD_Basic.h"
#include"AQU_GlobalTypes.h"
#include<d3dx9.h>
#include<d3d9.h>

/**
* Plugin node.
***/
class NOD_FrameDecalDx9 : public NOD_Basic 
{ 
public: 
	NOD_FrameDecalDx9(LONG nX, LONG nY);
	~NOD_FrameDecalDx9();

	/*** NOD_Basic public methods ***/
	virtual bool        SupportsD3DMethod(int eD3D, int eD3DInterface, int eD3DMethod);
	virtual void*       Provoke(void* pcThis, int eD3D, int eD3DInterface, int eD3DMethod, std::vector<NOD_Basic*>* ppaNodes);
	virtual const char* GetNodeType() { return "FrameDecal"; } 
	virtual UINT        GetNodeTypeId() { return D3DOPS_NODE_FRAMEDECAL; }

private:
	/**
	* The texture of the current frame.
	***/
	LPDIRECT3DTEXTURE9 m_pcFrameTexture9;
	/**
	* The surface of the current frame.
	***/
	LPDIRECT3DSURFACE9 m_pcFrameSurface9;
	/**
	* The surface description of the backbuffer.
	***/
	D3DSURFACE_DESC m_sDesc;
};

#endif