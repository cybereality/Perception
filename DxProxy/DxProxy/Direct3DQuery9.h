/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez
Modifications Copyright (C) 2013 Chris Drain, Denis Reischl, Neil Schneider 
  and Joshua Brown 

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

#ifndef DIRECT3DQUERY9_H_INCLUDED
#define DIRECT3DQUERY9_H_INCLUDED

#include <d3d9.h>
#include "Direct3DDevice9.h"

/**
*  Direct 3D query class. 
*  Overwrites IDirect3DQuery9 and imbeds the actual query.
*/
class BaseDirect3DQuery9 : public IDirect3DQuery9
{
public:
	BaseDirect3DQuery9(IDirect3DQuery9* pActualQuery, IDirect3DDevice9* pOwningDevice);
	virtual ~BaseDirect3DQuery9();

	/*** IUnknown methods ***/
	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv);
	virtual ULONG   WINAPI AddRef();
	virtual ULONG   WINAPI Release();

	/*** IDirect3DQuery9 methods ***/
	virtual D3DQUERYTYPE WINAPI GetType();
	virtual HRESULT	     WINAPI GetDevice(IDirect3DDevice9** ppDevice);
    virtual DWORD        WINAPI GetDataSize();
    virtual HRESULT      WINAPI Issue(DWORD dwIssueFlags);
    virtual HRESULT      WINAPI GetData(void* pData, DWORD dwSize, DWORD dwGetDataFlags);

protected:
	/**
	* The actual query embedded. 
	***/
	IDirect3DQuery9* const m_pActualQuery;
	/**
	* Pointer to the D3D device that owns the query. 
	***/
	IDirect3DDevice9* m_pOwningDevice;
	/**
	* Internal reference counter. 
	***/
	ULONG m_nRefCount;
};
#endif
