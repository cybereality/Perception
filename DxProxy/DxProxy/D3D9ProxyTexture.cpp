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

#include "D3D9ProxyTexture.h"
#include <assert.h>

D3D9ProxyTexture::D3D9ProxyTexture(IDirect3DTexture9* pActualTexture, BaseDirect3DDevice9* pOwningDevice) :
	BaseDirect3DTexture9(pActualTexture),
	m_wrappedSurfaceLevels(),
	m_pOwningDevice(pOwningDevice)
{
	assert (pOwningDevice != NULL);

	m_pOwningDevice->AddRef();
}

D3D9ProxyTexture::~D3D9ProxyTexture()
{
	// delete all surfaces in m_levels
	auto it = m_wrappedSurfaceLevels.begin();
	while (it != m_wrappedSurfaceLevels.end()) {
		// we have to explicitly delete the Surfaces here as the Release behaviour of the surface would get stuck in a loop
		// calling back to the container Release.
		delete it->second;
		it = m_wrappedSurfaceLevels.erase(it);
	}

	if (m_pOwningDevice)
		m_pOwningDevice->Release();
}









HRESULT WINAPI D3D9ProxyTexture::GetSurfaceLevel(UINT Level, IDirect3DSurface9** ppSurfaceLevel)
{
	// Have we already got a Proxy for this surface level?
	if (m_wrappedSurfaceLevels.count(Level) == 1) { // yes

		// TODO Should we call through to underlying texture and make sure the result of doing this operation on the 
		// underlying texture would still be a success? (not if we don't have to, will see if it becomes a problem)

		*ppSurfaceLevel = m_wrappedSurfaceLevels[Level];
		return D3D_OK;
	}
	else {
		// Get underlying surface, wrap, then store in m_wrappedSurfaceLevels and return the wrapped surface
		IDirect3DSurface9* pActualSurfaceLevel;
		HRESULT result = m_pActualTexture->GetSurfaceLevel(Level, &pActualSurfaceLevel);
		if (SUCCEEDED(result)) {

			D3D9ProxySurface* pWrappedSurfaceLevel = new D3D9ProxySurface(pActualSurfaceLevel, m_pOwningDevice, this);

			if(!m_wrappedSurfaceLevels.insert(std::pair<ULONG, D3D9ProxySurface*>(Level, pWrappedSurfaceLevel)).second) {
				// insertion of wrapped surface level into m_wrappedSurfaceLevels succeeded
				*ppSurfaceLevel = pWrappedSurfaceLevel;
				return D3D_OK;
			}
			else {
				// Failure to insert should not be possible. In this case we could still return the wrapped surface,
				// however, if we did and it was requested again a new wrapped instance will be returned and things would explode
				// at some point. Better to fail fast.
				OutputDebugString(__FUNCTION__);
				OutputDebugString("\n");
				OutputDebugString("Unable to store surface level\n");
				assert(false);

				return D3DERR_INVALIDCALL;
			}
		}
		else { 
			OutputDebugString(__FUNCTION__);
			OutputDebugString("\n");
			OutputDebugString("Error fetching actual surface level.\n");
			return result;
		}
	}

	//return m_pActualTexture->GetSurfaceLevel(Level, ppSurfaceLevel);
}






HRESULT WINAPI D3D9ProxyTexture::GetDevice(IDirect3DDevice9** ppDevice)
{
	if (!m_pOwningDevice)
		return D3DERR_INVALIDCALL;
	else {
		*ppDevice = m_pOwningDevice;
		m_pOwningDevice->AddRef();
		return D3D_OK;
	}
}
