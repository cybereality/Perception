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

#include "D3D9ProxyStateBlock.h"
#include <assert.h>

D3D9ProxyStateBlock::D3D9ProxyStateBlock(IDirect3DStateBlock9* pActualStateBlock, D3DProxyDevice *pOwningDevice, int CaptureType) :
	BaseDirect3DStateBlock9(pActualStateBlock, pOwningDevice)
{

}

D3D9ProxyStateBlock::~D3D9ProxyStateBlock()
{

}




HRESULT WINAPI D3D9ProxyStateBlock::Capture()
{

	// TODO
	// Capture on actual device
	// For all actively tracked special states capture from proxy device 
	//  this could be none (tracked states are all simple and handled by device), it could be a collection of individual states, 
	//  all states, vertex states or pixel states)


	return m_pActualStateBlock->Capture();
}

HRESULT WINAPI D3D9ProxyStateBlock::Apply()
{
	// TODO
	// Apply on actual device.
	// If mixed sides then manually apply all state from stereo components based on the current device side

	// Update the proxy device 


	return m_pActualStateBlock->Apply();
}