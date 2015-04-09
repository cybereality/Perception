/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Stereo Splitter - Vireio Perception Render Target Handler
Copyright (C) 2015 Denis Reischl

File <VireioStereoSplitter.h> and
Class <VireioStereoSplitter> :
Copyright (C) 2015 Denis Reischl

The stub class <AQU_Nodus> is the only public class from the Aquilinus 
repository and permitted to be used for open source plugins of any kind. 
Read the Aquilinus documentation for further information.

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 onwards 2014 by Grant Bagwell, Simon Brown and Neil Schneider

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

#include"AQU_Nodus.h"
#include"Resources.h"

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#define PNT_FLOAT_PLUG_TYPE                          104
#define PNT_INT_PLUG_TYPE                            107 
#define PNT_UINT_PLUG_TYPE                           112

#define NUMBER_OF_DECOMMANDERS                        25

/**
* Node Commander Enumeration.
***/
enum STS_Decommanders
{
	RenderTargetIndex,            /**< ->SetRenderTarget() render target index ***/
	pRenderTarget,                /**< ->SetRenderTarget() render target ***/
	Sampler,                      /**< ->SetTexture() sampler index **/
	pTexture,                     /**< ->SetTexture() texture pointer ***/
	PrimitiveType,                /**< ->DrawPrimitive() primitive type ***/
	StartVertex,                  /**< ->DrawPrimitive() start vertex ***/
	PrimitiveCount,               /**< ->DrawPrimitive() primitive count ***/
	Type,                         /**< ->DrawIndexedPrimitive() primitive type ***/
	BaseVertexIndex,              /**< ->DrawIndexedPrimitive() base vertex index ***/
	MinIndex,                     /**< ->DrawIndexedPrimitive() minimum vertex index ***/
	NumVertices,                  /**< ->DrawIndexedPrimitive() number of vertices ***/
	StartIndex,                   /**< ->DrawIndexedPrimitive() start index  ***/
	PrimitiveCountIndexed,        /**< ->DrawIndexedPrimitive() primitive count ***/
	PrimitiveTypeUP,              /**< ->DrawPrimitiveUP() primitive type ***/
	PrimitiveCountUP,             /**< ->DrawPrimitiveUP() primitive count ***/
	pVertexStreamZeroData,        /**< ->DrawPrimitiveUP() memory pointer to the vertex data ***/
	VertexStreamZeroStride,       /**< ->DrawPrimitiveUP() number of bytes of data for each vertex ***/
	PrimitiveTypeUPIndexed,       /**< ->DrawIndexedPrimitiveUP() primitive type ***/
	MinVertexIndex,               /**< ->DrawIndexedPrimitiveUP() minimum vertex index ***/
	NumVerticesUPIndexed,         /**< ->DrawIndexedPrimitiveUP() number of vertices ***/
	PrimitiveCountUPIndexed,      /**< ->DrawIndexedPrimitiveUP() primitive count ***/
	pIndexData,                   /**< ->DrawIndexedPrimitiveUP() memory pointer to the index data ***/
	IndexDataFormat,              /**< ->DrawIndexedPrimitiveUP() format of the index data ***/
	pVertexStreamZeroDataIndexed, /**< ->DrawIndexedPrimitiveUP() memory pointer to the vertex data ***/
	VertexStreamZeroStrideIndexed /**< ->DrawIndexedPrimitiveUP() number of bytes of data for each vertex ***/
};

/**
* Vireio Stereo Splitter Node Plugin (Direct3D 9).
* Vireio Perception Stereo Render Target Handler.
***/
class StereoSplitter : public AQU_Nodus
{
public:
	StereoSplitter();
	virtual ~StereoSplitter();

	/*** AQU_Nodus public methods ***/
	virtual const char*     GetNodeType();
	virtual UINT            GetNodeTypeId();
	virtual LPWSTR          GetCategory();
	virtual HBITMAP         GetLogo();
	virtual HBITMAP         GetControl();
	virtual DWORD           GetNodeWidth() { return 4+256+4; }
	virtual DWORD           GetNodeHeight() { return 128; }
	virtual DWORD           GetDecommandersNumber() { return NUMBER_OF_DECOMMANDERS; }
	virtual LPWSTR          GetDecommanderName(DWORD dwDecommanderIndex);
	virtual DWORD           GetDecommanderType(DWORD dwDecommanderIndex);
	virtual void            SetInputPointer(DWORD dwDecommanderIndex, void* pData);
	virtual bool            SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod);
	virtual void*           Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex);
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	StereoSplitter* pStereoSplitter = new StereoSplitter();
	return static_cast<AQU_Nodus*>(pStereoSplitter);
}
