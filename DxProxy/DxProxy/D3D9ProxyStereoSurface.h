/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2013 Chris Drain

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

#ifndef D3DPROXYSTEREOSURFACE_H_INCLUDED
#define D3DPROXYSTEREOSURFACE_H_INCLUDED

#include <d3d9.h>
#include "Direct3DSurface9.h"
#include "Direct3DDevice9.h"
#include "D3D9ProxySurface.h"
#include <stdio.h>

/*
		


 */
class D3D9ProxyStereoSurface : public D3D9ProxySurface
{
public:
	D3D9ProxyStereoSurface(IDirect3DSurface9* pActualSurfaceLeft, IDirect3DSurface9* pActualSurfaceRight, BaseDirect3DDevice9* pOwningDevice, IUnknown* pStereoContainer);
	virtual ~D3D9ProxyStereoSurface();

	bool IsStereo();

	IDirect3DSurface9* getMonoSurface();
	IDirect3DSurface9* getLeftSurface();
	IDirect3DSurface9* getRightSurface();


	

	virtual HRESULT WINAPI SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags);
	virtual HRESULT WINAPI FreePrivateData(REFGUID refguid);
	virtual   DWORD WINAPI SetPriority(DWORD PriorityNew);
	virtual    void WINAPI PreLoad();

	


	virtual HRESULT WINAPI LockRect(D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags);
	virtual HRESULT WINAPI UnlockRect();
	virtual HRESULT WINAPI ReleaseDC(HDC hdc);

private:

	IDirect3DSurface9* const m_pActualSurfaceRight;
};

#endif
