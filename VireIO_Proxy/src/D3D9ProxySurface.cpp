/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3D9ProxySurface.cpp> and
Class <D3D9ProxySurface> :
Copyright (C) 2013 Chris Drain

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

#include "D3D9ProxySurface.h"


D3D9ProxySurface::D3D9ProxySurface(IDirect3DSurface9* pActualSurfaceLeft, IDirect3DSurface9* pActualSurfaceRight, D3DProxyDevice* pOwningDevice, IUnknown* pWrappedContainer) :
	cBase( pActualSurfaceLeft , pOwningDevice , pWrappedContainer ) ,
	right(pActualSurfaceRight)
{
}


D3D9ProxySurface::~D3D9ProxySurface(){
	if( right ){
		right->Release();
	}
}


HRESULT WINAPI D3D9ProxySurface::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags){
	if( right ){
		right->SetPrivateData(refguid, pData, SizeOfData, Flags);
	}

	return actual->SetPrivateData(refguid, pData, SizeOfData, Flags);
}


HRESULT WINAPI D3D9ProxySurface::GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData){
	return actual->GetPrivateData(refguid, pData, pSizeOfData);
}


HRESULT WINAPI D3D9ProxySurface::FreePrivateData(REFGUID refguid){
	if( right ){
		right->FreePrivateData(refguid);
	}

	return actual->FreePrivateData(refguid);
}


DWORD WINAPI D3D9ProxySurface::SetPriority(DWORD PriorityNew){
	if( right ){
		right->SetPriority(PriorityNew);
	}

	return actual->SetPriority(PriorityNew);
}


DWORD WINAPI D3D9ProxySurface::GetPriority(){
	return actual->GetPriority();
}


D3DRESOURCETYPE WINAPI D3D9ProxySurface::GetType(){
	return actual->GetType();
}


HRESULT WINAPI D3D9ProxySurface::GetDesc(D3DSURFACE_DESC *pDesc){
	return actual->GetDesc(pDesc);
}


HRESULT WINAPI D3D9ProxySurface::GetDC(HDC *phdc){
	return actual->GetDC(phdc);
}


void WINAPI D3D9ProxySurface::PreLoad(){
	if( right ){
		right->PreLoad();
	}

	return actual->PreLoad();
}


HRESULT WINAPI D3D9ProxySurface::LockRect(D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags){
	if( right ){
		right->LockRect(pLockedRect, pRect, Flags);
	}

	return actual->LockRect(pLockedRect, pRect, Flags);
}


HRESULT WINAPI D3D9ProxySurface::UnlockRect(){
	if( right ){
		right->UnlockRect();
	}

	return actual->UnlockRect();
}


HRESULT WINAPI D3D9ProxySurface::ReleaseDC(HDC hdc){
	if( right ){
		right->ReleaseDC(hdc);
	}

	return actual->ReleaseDC(hdc);
}
