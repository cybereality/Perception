/***************************************************************
Aquilinus - 3D Modification Studio
Copyright © 2014 Denis Reischl

File <PCL_ID3D10DeviceChild.h> and
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

#ifndef PCL_ID3D10DEVICECHILD_CLASS
#define PCL_ID3D10DEVICECHILD_CLASS

#include <D3D10.h>

/**
* Wrapped ID3D10DeviceChild class, or proxy class.
***/
class PCL_ID3D10DeviceChild : public ID3D10DeviceChild
{
public:
	PCL_ID3D10DeviceChild(ID3D10DeviceChild* pID3D10DeviceChild);
	virtual ~PCL_ID3D10DeviceChild(void);

	/*** IUnknown methods ***/
	virtual HRESULT WINAPI QueryInterface          (REFIID riid, void **ppvObject);
	virtual ULONG   WINAPI AddRef                  (void);
	virtual ULONG   WINAPI Release                 (void);

	/*** ID3D10DeviceChild methods ***/
	virtual void    WINAPI GetDevice               (ID3D10Device **ppDevice);
	virtual HRESULT WINAPI GetPrivateData          (REFGUID guid, UINT *pDataSize, void *pData);
	virtual HRESULT WINAPI SetPrivateData          (REFGUID guid, UINT DataSize, const void *pData);
	virtual HRESULT WINAPI SetPrivateDataInterface (REFGUID guid, const IUnknown *pData);

protected:
	/**
	* The actual D3D10 device child.
	***/
	ID3D10DeviceChild* m_pID3D10DeviceChild;
};

#endif