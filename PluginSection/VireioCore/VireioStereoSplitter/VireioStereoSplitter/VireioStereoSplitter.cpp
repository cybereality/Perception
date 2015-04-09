/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Stereo Splitter - Vireio Perception Render Target Handler
Copyright (C) 2015 Denis Reischl

File <VireioStereoSplitter.cpp> and
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

#include"VireioStereoSplitter.h"

#define INTERFACE_IDIRECT3DDEVICE9                           8
#define INTERFACE_IDIRECT3DSWAPCHAIN9                       15
#define	METHOD_IDIRECT3DDEVICE9_PRESENT                     17
#define METHOD_IDIRECT3DDEVICE9_SETRENDERTARGET             37
#define METHOD_IDIRECT3DDEVICE9_SETTEXTURE                  65
#define METHOD_IDIRECT3DDEVICE9_DRAWPRIMITIVE               81
#define METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVE        82 
#define METHOD_IDIRECT3DDEVICE9_DRAWPRIMITIVEUP             83 
#define METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVEUP      84 
#define	METHOD_IDIRECT3DSWAPCHAIN9_PRESENT                   3

/**
* Constructor.
***/
StereoSplitter::StereoSplitter():AQU_Nodus()
{
}

/**
* Destructor.
***/
StereoSplitter::~StereoSplitter()
{
}

/**
* Return the name of the  Stereo Splitter node.
***/
const char* StereoSplitter::GetNodeType() 
{
	return "Stereo Splitter"; 
}

/**
* Returns a global unique identifier for the Stereo Splitter node.
***/
UINT StereoSplitter::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#define MY_PLUGIN_IDENTIFIER 64
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/**
* Returns the name of the category for the Stereo Splitter node.
***/
LPWSTR StereoSplitter::GetCategory()
{
	return L"Vireio Core";
}

/**
* Returns a logo to be used for the Stereo Splitter node.
***/
HBITMAP StereoSplitter::GetLogo() 
{ 
	HMODULE hModule = GetModuleHandle(L"VireioStereoSplitter.dll");
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/** 
* Returns the updated control for the Stereo Splitter node.
* Allways return >nullptr< if there is no update for the control !!
***/
HBITMAP StereoSplitter::GetControl()
{
	return nullptr;
}

/**
* Provides the name of the requested decommander.
***/
LPWSTR StereoSplitter::GetDecommanderName(DWORD dwDecommanderIndex)
{
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
	case STS_Decommanders::RenderTargetIndex:             /**< ->SetRenderTarget() render target index ***/
		return L"RenderTargetIndex";
	case STS_Decommanders::pRenderTarget:                 /**< ->SetRenderTarget() render target ***/
		return L"pRenderTarget";
	case STS_Decommanders::Sampler:                       /**< ->SetTexture() sampler index **/
		return L"Sampler";
	case STS_Decommanders::pTexture:                      /**< ->SetTexture() texture pointer ***/
		return L"pTexture";
	case STS_Decommanders::PrimitiveType:                 /**< ->DrawPrimitive() primitive type ***/
		return L"PrimitiveType";
	case STS_Decommanders::StartVertex:                   /**< ->DrawPrimitive() start vertex ***/
		return L"StartVertex";
	case STS_Decommanders::PrimitiveCount:                /**< ->DrawPrimitive() primitive count ***/
		return L"PrimitiveCount";
	case STS_Decommanders::Type:                          /**< ->DrawIndexedPrimitive() primitive type ***/
		return L"Type";
	case STS_Decommanders::BaseVertexIndex:               /**< ->DrawIndexedPrimitive() base vertex index ***/
		return L"BaseVertexIndex";
	case STS_Decommanders::MinIndex:                      /**< ->DrawIndexedPrimitive() minimum vertex index ***/
		return L"MinIndex";
	case STS_Decommanders::NumVertices:                   /**< ->DrawIndexedPrimitive() number of vertices ***/
		return L"NumVertices";
	case STS_Decommanders::StartIndex:                    /**< ->DrawIndexedPrimitive() start index  ***/
		return L"StartIndex";
	case STS_Decommanders::PrimitiveCountIndexed:         /**< ->DrawIndexedPrimitive() primitive count ***/
		return L"PrimitiveCountIndexed";
	case STS_Decommanders::PrimitiveTypeUP:               /**< ->DrawPrimitiveUP() primitive type ***/
		return L"PrimitiveTypeUP";
	case STS_Decommanders::PrimitiveCountUP:              /**< ->DrawPrimitiveUP() primitive count ***/
		return L"PrimitiveCountUP";
	case STS_Decommanders::pVertexStreamZeroData:         /**< ->DrawPrimitiveUP() memory pointer to the vertex data ***/
		return L"pVertexStreamZeroData";
	case STS_Decommanders::VertexStreamZeroStride:        /**< ->DrawPrimitiveUP() number of bytes of data for each vertex ***/
		return L"VertexStreamZeroStride";
	case STS_Decommanders::PrimitiveTypeUPIndexed:        /**< ->DrawIndexedPrimitiveUP() primitive type ***/
		return L"PrimitiveTypeUPIndexed";
	case STS_Decommanders::MinVertexIndex:                /**< ->DrawIndexedPrimitiveUP() minimum vertex index ***/
		return L"MinVertexIndex";
	case STS_Decommanders::NumVerticesUPIndexed:          /**< ->DrawIndexedPrimitiveUP() number of vertices ***/
		return L"NumVerticesUPIndexed";
	case STS_Decommanders::PrimitiveCountUPIndexed:       /**< ->DrawIndexedPrimitiveUP() primitive count ***/
		return L"PrimitiveCountUPIndexed";
	case STS_Decommanders::pIndexData:                    /**< ->DrawIndexedPrimitiveUP() memory pointer to the index data ***/
		return L"pIndexData";
	case STS_Decommanders::IndexDataFormat:               /**< ->DrawIndexedPrimitiveUP() format of the index data ***/
		return L"IndexDataFormat";
	case STS_Decommanders::pVertexStreamZeroDataIndexed:  /**< ->DrawIndexedPrimitiveUP() memory pointer to the vertex data ***/
		return L"pVertexStreamZeroDataIndexed";
	case STS_Decommanders::VertexStreamZeroStrideIndexed: /**< ->DrawIndexedPrimitiveUP() number of bytes of data for each vertex ***/
		return L"VertexStreamZeroStrideIndexed";
	}

	return L"";
}

/**
* Returns the plug type for the requested decommander.
***/
DWORD StereoSplitter::GetDecommanderType(DWORD dwDecommanderIndex) 
{
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
	case STS_Decommanders::RenderTargetIndex:             /**< ->SetRenderTarget() render target index ***/
	case STS_Decommanders::pRenderTarget:                 /**< ->SetRenderTarget() render target ***/
	case STS_Decommanders::Sampler:                       /**< ->SetTexture() sampler index **/
	case STS_Decommanders::pTexture:                      /**< ->SetTexture() texture pointer ***/
	case STS_Decommanders::PrimitiveType:                 /**< ->DrawPrimitive() primitive type ***/
	case STS_Decommanders::StartVertex:                   /**< ->DrawPrimitive() start vertex ***/
	case STS_Decommanders::PrimitiveCount:                /**< ->DrawPrimitive() primitive count ***/
	case STS_Decommanders::Type:                          /**< ->DrawIndexedPrimitive() primitive type ***/
	case STS_Decommanders::BaseVertexIndex:               /**< ->DrawIndexedPrimitive() base vertex index ***/
	case STS_Decommanders::MinIndex:                      /**< ->DrawIndexedPrimitive() minimum vertex index ***/
	case STS_Decommanders::NumVertices:                   /**< ->DrawIndexedPrimitive() number of vertices ***/
	case STS_Decommanders::StartIndex:                    /**< ->DrawIndexedPrimitive() start index  ***/
	case STS_Decommanders::PrimitiveCountIndexed:         /**< ->DrawIndexedPrimitive() primitive count ***/
	case STS_Decommanders::PrimitiveTypeUP:               /**< ->DrawPrimitiveUP() primitive type ***/
	case STS_Decommanders::PrimitiveCountUP:              /**< ->DrawPrimitiveUP() primitive count ***/
	case STS_Decommanders::pVertexStreamZeroData:         /**< ->DrawPrimitiveUP() memory pointer to the vertex data ***/
	case STS_Decommanders::VertexStreamZeroStride:        /**< ->DrawPrimitiveUP() number of bytes of data for each vertex ***/
	case STS_Decommanders::PrimitiveTypeUPIndexed:        /**< ->DrawIndexedPrimitiveUP() primitive type ***/
	case STS_Decommanders::MinVertexIndex:                /**< ->DrawIndexedPrimitiveUP() minimum vertex index ***/
	case STS_Decommanders::NumVerticesUPIndexed:          /**< ->DrawIndexedPrimitiveUP() number of vertices ***/
	case STS_Decommanders::PrimitiveCountUPIndexed:       /**< ->DrawIndexedPrimitiveUP() primitive count ***/
	case STS_Decommanders::pIndexData:                    /**< ->DrawIndexedPrimitiveUP() memory pointer to the index data ***/
	case STS_Decommanders::IndexDataFormat:               /**< ->DrawIndexedPrimitiveUP() format of the index data ***/
	case STS_Decommanders::pVertexStreamZeroDataIndexed:  /**< ->DrawIndexedPrimitiveUP() memory pointer to the vertex data ***/
	case STS_Decommanders::VertexStreamZeroStrideIndexed: /**< ->DrawIndexedPrimitiveUP() number of bytes of data for each vertex ***/
		return 0;
		//return PNT_IDIRECT3DTEXTURE9_PLUG_TYPE;
	}

	return 0;
}

/**
* Sets the input pointer for the requested decommander.
***/
void StereoSplitter::SetInputPointer(DWORD dwDecommanderIndex, void* pData)
{
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
	case STS_Decommanders::RenderTargetIndex:             /**< ->SetRenderTarget() render target index ***/
	case STS_Decommanders::pRenderTarget:                 /**< ->SetRenderTarget() render target ***/
	case STS_Decommanders::Sampler:                       /**< ->SetTexture() sampler index **/
	case STS_Decommanders::pTexture:                      /**< ->SetTexture() texture pointer ***/
	case STS_Decommanders::PrimitiveType:                 /**< ->DrawPrimitive() primitive type ***/
	case STS_Decommanders::StartVertex:                   /**< ->DrawPrimitive() start vertex ***/
	case STS_Decommanders::PrimitiveCount:                /**< ->DrawPrimitive() primitive count ***/
	case STS_Decommanders::Type:                          /**< ->DrawIndexedPrimitive() primitive type ***/
	case STS_Decommanders::BaseVertexIndex:               /**< ->DrawIndexedPrimitive() base vertex index ***/
	case STS_Decommanders::MinIndex:                      /**< ->DrawIndexedPrimitive() minimum vertex index ***/
	case STS_Decommanders::NumVertices:                   /**< ->DrawIndexedPrimitive() number of vertices ***/
	case STS_Decommanders::StartIndex:                    /**< ->DrawIndexedPrimitive() start index  ***/
	case STS_Decommanders::PrimitiveCountIndexed:         /**< ->DrawIndexedPrimitive() primitive count ***/
	case STS_Decommanders::PrimitiveTypeUP:               /**< ->DrawPrimitiveUP() primitive type ***/
	case STS_Decommanders::PrimitiveCountUP:              /**< ->DrawPrimitiveUP() primitive count ***/
	case STS_Decommanders::pVertexStreamZeroData:         /**< ->DrawPrimitiveUP() memory pointer to the vertex data ***/
	case STS_Decommanders::VertexStreamZeroStride:        /**< ->DrawPrimitiveUP() number of bytes of data for each vertex ***/
	case STS_Decommanders::PrimitiveTypeUPIndexed:        /**< ->DrawIndexedPrimitiveUP() primitive type ***/
	case STS_Decommanders::MinVertexIndex:                /**< ->DrawIndexedPrimitiveUP() minimum vertex index ***/
	case STS_Decommanders::NumVerticesUPIndexed:          /**< ->DrawIndexedPrimitiveUP() number of vertices ***/
	case STS_Decommanders::PrimitiveCountUPIndexed:       /**< ->DrawIndexedPrimitiveUP() primitive count ***/
	case STS_Decommanders::pIndexData:                    /**< ->DrawIndexedPrimitiveUP() memory pointer to the index data ***/
	case STS_Decommanders::IndexDataFormat:               /**< ->DrawIndexedPrimitiveUP() format of the index data ***/
	case STS_Decommanders::pVertexStreamZeroDataIndexed:  /**< ->DrawIndexedPrimitiveUP() memory pointer to the vertex data ***/
	case STS_Decommanders::VertexStreamZeroStrideIndexed: /**< ->DrawIndexedPrimitiveUP() number of bytes of data for each vertex ***/
		break;
	}
}

/**
* Stereo Splitter supports various D3D9 calls.
***/
bool StereoSplitter::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)  
{ 
	if ((nD3DVersion >= (int)AQU_DirectXVersion::DirectX_9_0) &&
		(nD3DVersion <= (int)AQU_DirectXVersion::DirectX_9_29))
	{
		if (nD3DInterface == INTERFACE_IDIRECT3DDEVICE9)
		{
			if ((nD3DMethod == METHOD_IDIRECT3DDEVICE9_PRESENT) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_SETRENDERTARGET) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_SETTEXTURE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_DRAWPRIMITIVE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVE) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_DRAWPRIMITIVEUP) ||
				(nD3DMethod == METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVEUP))
				return true;
		}
		if (nD3DInterface == INTERFACE_IDIRECT3DSWAPCHAIN9)
		{
			if (nD3DMethod == METHOD_IDIRECT3DSWAPCHAIN9_PRESENT) return true;
		}
	}
	return false; 
}

/**
* Handle Stereo Render Targets.
***/
void* StereoSplitter::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)	
{
	switch(eD3DInterface)
	{
	case INTERFACE_IDIRECT3DDEVICE9:
		switch(eD3DMethod)
		{
		case METHOD_IDIRECT3DDEVICE9_PRESENT:
			return nullptr;
		case METHOD_IDIRECT3DDEVICE9_SETRENDERTARGET:
			return nullptr;
		case METHOD_IDIRECT3DDEVICE9_SETTEXTURE:
			return nullptr;
		case METHOD_IDIRECT3DDEVICE9_DRAWPRIMITIVE:
			return nullptr;
		case METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVE:
			return nullptr;
		case METHOD_IDIRECT3DDEVICE9_DRAWPRIMITIVEUP:
			return nullptr;
		case METHOD_IDIRECT3DDEVICE9_DRAWINDEXEDPRIMITIVEUP:
			return nullptr;
		}
		return nullptr;
	case INTERFACE_IDIRECT3DSWAPCHAIN9:
		switch(eD3DMethod)
		{
		case METHOD_IDIRECT3DSWAPCHAIN9_PRESENT:
			return nullptr;
		}
		return nullptr;
	}
	return nullptr;
}