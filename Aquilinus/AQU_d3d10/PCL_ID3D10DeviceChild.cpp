/***************************************************************
Aquilinus - 3D Modification Studio
Copyright © 2014 Denis Reischl

File <PCL_ID3D10DeviceChild.cpp> and
Class <PCL_ID3D10DeviceChild> :
Copyright © 2014 Denis Reischl

This code is private and MUST NOT be set public for any 
reason. This code is intended to be used, changed, compiled 
and its build published only by Denis Reischl.

This code is intended to be used by its author,
Denis Reischl,
for any commercial and non-commercial purpose.

Following persons are granted to read the full Aquilinus
source code for life time :
Neil Schneider, Grant Bagwell and Simon Brown.
If the original author of Aquilinus, Denis Reischl,
discontinues the work on this software, the named persons
automatically inherit all rights to continue this work.

Except where otherwise noted, this work is licensed under :
<http://creativecommons.org/licenses/by-nc-nd/3.0/deed.en_US>.
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