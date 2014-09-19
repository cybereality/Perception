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


METHOD_THRU_LR( HRESULT         , WINAPI , D3D9ProxySurface , SetPrivateData , REFGUID , refguid , CONST void* , pData , DWORD , SizeOfData , DWORD , Flags)
METHOD_THRU   ( HRESULT         , WINAPI , D3D9ProxySurface , GetPrivateData , REFGUID , refguid , void* , pData , DWORD* , pSizeOfData )
METHOD_THRU_LR( HRESULT         , WINAPI , D3D9ProxySurface , FreePrivateData , REFGUID , refguid )
METHOD_THRU_LR( DWORD           , WINAPI , D3D9ProxySurface , SetPriority , DWORD , PriorityNew )
METHOD_THRU   ( DWORD           , WINAPI , D3D9ProxySurface , GetPriority )
METHOD_THRU   ( D3DRESOURCETYPE , WINAPI , D3D9ProxySurface , GetType )
METHOD_THRU   ( HRESULT         , WINAPI , D3D9ProxySurface , GetDesc , D3DSURFACE_DESC* , pDesc )
METHOD_THRU   ( HRESULT         , WINAPI , D3D9ProxySurface , GetDC , HDC* , phdc )
METHOD_THRU_LR( void            , WINAPI , D3D9ProxySurface , PreLoad )
METHOD_THRU_LR( HRESULT         , WINAPI , D3D9ProxySurface , LockRect , D3DLOCKED_RECT* , pLockedRect , CONST RECT* , pRect , DWORD , Flags )
METHOD_THRU_LR( HRESULT         , WINAPI , D3D9ProxySurface , UnlockRect )
METHOD_THRU_LR( HRESULT         , WINAPI , D3D9ProxySurface , ReleaseDC , HDC , hdc )
