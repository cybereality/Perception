/***************************************************************
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
****************************************************************/

#include "PCL_ID3D10DeviceChild.h"

/**
* Constructor.
***/
PCL_ID3D10DeviceChild::PCL_ID3D10DeviceChild(ID3D10DeviceChild* pID3D10DeviceChild) 
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("PCL_ID3D10DeviceChild");
#endif

	m_pID3D10DeviceChild = pID3D10DeviceChild;
}

/**
* Empty deconstructor.
***/
PCL_ID3D10DeviceChild::~PCL_ID3D10DeviceChild(void) 
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("~PCL_ID3D10DeviceChild");
#endif

}

#define IF_GUID(riid,a,b,c,d,e,f,g,h,i,j,k) if ((riid.Data1==a)&&(riid.Data2==b)&&(riid.Data3==c)&&(riid.Data4[0]==d)&&(riid.Data4[1]==e)&&(riid.Data4[2]==f)&&(riid.Data4[3]==g)&&(riid.Data4[4]==h)&&(riid.Data4[5]==i)&&(riid.Data4[6]==j)&&(riid.Data4[7]==k))
/**
* Wrapped QueryInterface functionality.
***/
HRESULT PCL_ID3D10DeviceChild::QueryInterface(REFIID riid, void **ppvObject) 
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("QueryInterface");
#endif

	return m_pID3D10DeviceChild->QueryInterface(riid, ppvObject);
}

/**
*
***/
ULONG PCL_ID3D10DeviceChild::AddRef(void) 
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("AddRef");
#endif

	return m_pID3D10DeviceChild->AddRef(); 
}

/**
*
***/
ULONG PCL_ID3D10DeviceChild::Release(void) 
{ 
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("Release");
#endif

	ULONG count = m_pID3D10DeviceChild->Release();

	if (count == 0)
		delete this;

	return count;
}

/**
*
***/
void WINAPI PCL_ID3D10DeviceChild::GetDevice(ID3D10Device **ppDevice)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GetDevice");
#endif
	// TODO !! return proxy device
	return m_pID3D10DeviceChild->GetDevice(ppDevice);
}

/**
*
***/
HRESULT WINAPI PCL_ID3D10DeviceChild::GetPrivateData(REFGUID guid, UINT *pDataSize, void *pData)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GetPrivateData");
#endif
	return m_pID3D10DeviceChild->GetPrivateData(guid, pDataSize, pData);
}

/**
*
***/
HRESULT WINAPI PCL_ID3D10DeviceChild::SetPrivateData(REFGUID guid, UINT DataSize, const void *pData)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("SetPrivateData");
#endif
	return m_pID3D10DeviceChild->SetPrivateData(guid, DataSize, pData);
}

/**
*
***/
HRESULT WINAPI PCL_ID3D10DeviceChild::SetPrivateDataInterface(REFGUID guid, const IUnknown *pData)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("SetPrivateDataInterface");
#endif
	return m_pID3D10DeviceChild->SetPrivateDataInterface(guid, pData);
}