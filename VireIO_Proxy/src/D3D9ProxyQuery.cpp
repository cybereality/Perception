/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Direct3DQuery9.cpp> and
Class <D3D9ProxyQuery> :
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

#include "D3D9ProxyQuery.h"

D3D9ProxyQuery::D3D9ProxyQuery(IDirect3DQuery9* pActualQuery, D3DProxyDevice* pOwningDevice) :
	cBase( pActualQuery , pOwningDevice )
{
	assert (pActualQuery != NULL);
	assert (pOwningDevice != NULL);
}


DWORD WINAPI D3D9ProxyQuery::GetDataSize(){
	return actual->GetDataSize();
}


HRESULT WINAPI D3D9ProxyQuery::Issue(DWORD dwIssueFlags){
	return actual->Issue(dwIssueFlags);
}


HRESULT WINAPI D3D9ProxyQuery::GetData(void* pData, DWORD dwSize, DWORD dwGetDataFlags){
	return actual->GetData(pData, dwSize, dwGetDataFlags);
}


D3DQUERYTYPE WINAPI D3D9ProxyQuery::GetType(){
	return actual->GetType();
}