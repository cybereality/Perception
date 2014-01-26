/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3DProxyDeviceAdv.cpp> and
Class <D3DProxyDeviceAdv> :
Copyright (C) 2012 Andres Hernandez
Modifications (C) 2014 Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown

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

#include "D3DProxyDeviceAdv.h"

/**
* Constructor.
* @param pDevice Imbed actual device.
* @param pCreatedBy Pointer to the object that created the device.
***/
D3DProxyDeviceAdv::D3DProxyDeviceAdv(IDirect3DDevice9* pDevice, BaseDirect3D9* pCreatedBy) : D3DProxyDevice(pDevice, pCreatedBy)
{ }

/**
* Destructor.
***/
D3DProxyDeviceAdv::~D3DProxyDeviceAdv()
{ }

/**
* Set scissor test render state to false before draw.
***/
HRESULT WINAPI D3DProxyDeviceAdv::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount)
{
	SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	return D3DProxyDevice::DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

/**
* Set scissor test render state to false before draw.
***/
HRESULT WINAPI D3DProxyDeviceAdv::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
	if (NumVertices==4) setDrawingSide(vireio::Left);
	SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	return D3DProxyDevice::DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

/**
* Set scissor test render state to false before draw.
***/
HRESULT WINAPI D3DProxyDeviceAdv::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	return D3DProxyDevice::DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

/**
* Set scissor test render state to false before draw.
***/
HRESULT WINAPI D3DProxyDeviceAdv::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	return D3DProxyDevice::DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}