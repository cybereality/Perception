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