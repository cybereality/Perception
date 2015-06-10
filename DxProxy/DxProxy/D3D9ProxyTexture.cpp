/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3D9ProxyTexture.cpp> and
Class <D3D9ProxyTexture> :
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

#include "D3D9ProxyTexture.h"
#include <assert.h>

/**
* Constructor.
* @see D3D9ProxySurface::D3D9ProxySurface
***/
D3D9ProxyTexture::D3D9ProxyTexture(UINT Width, UINT Height, D3DFORMAT Format, bool lockable, IDirect3DTexture9* pActualTextureLeft, IDirect3DTexture9* pActualTextureRight, BaseDirect3DDevice9* pOwningDevice) :
	BaseDirect3DTexture9(pActualTextureLeft),
	m_pActualTextureRight(pActualTextureRight),
	m_wrappedSurfaceLevels(),
	m_pOwningDevice(pOwningDevice),
	m_Width(Width),
	m_Height(Height),
	m_Format(Format),
	m_lockable(lockable)
{
	#ifdef SHOW_CALLS
		OutputDebugString("called D3D9ProxyTexture::D3D9ProxyTexture");
	#endif
	assert (pOwningDevice != NULL);

	m_pOwningDevice->AddRef();
}

/**
* Destructor.
* Deletes wrapped surfaces, releases textures.
***/
D3D9ProxyTexture::~D3D9ProxyTexture()
{
	#ifdef SHOW_CALLS
		OutputDebugString("called D3D9ProxyTexture::~D3D9ProxyTexture");
	#endif
	// delete all surfaces in m_levels
	auto it = m_wrappedSurfaceLevels.begin();
	while (it != m_wrappedSurfaceLevels.end()) {
		// we have to explicitly delete the Surfaces here as the Release behaviour of the surface would get stuck in a loop
		// calling back to the container Release.
		delete it->second;
		it = m_wrappedSurfaceLevels.erase(it);
	}

	if (m_pActualTextureRight)
		m_pActualTextureRight->Release();

	if (m_pOwningDevice)
		m_pOwningDevice->Release();
}

#define IF_GUID(riid,a,b,c,d,e,f,g) if ((riid.Data1==a)&&(riid.Data2==b)&&(riid.Data3==c)&&(riid.Data4[0]==d)&&(riid.Data4[1]==e)&&(riid.Data4[2]==f)&&(riid.Data4[3]==g))
/**
* Ensures Skyrim works (and maybe other games).
* Skyrim sometimes calls QueryInterface() to get the underlying surface instead of GetSurfaceLevel().
* It even calls it to query the texture class.
* @return (this) in case of query texture interface, GetSurfaceLevel() in case of query surface.
***/
HRESULT WINAPI D3D9ProxyTexture::QueryInterface(REFIID riid, LPVOID* ppv)
{
	#ifdef SHOW_CALLS
		OutputDebugString("called D3D9ProxyTexture::QueryInterface");
	#endif
	/* IID_IDirect3DTexture9 */
	/* {85C31227-3DE5-4f00-9B3A-F11AC38C18B5} */
	IF_GUID(riid,0x85c31227,0x3de5,0x4f00,0x9b,0x3a,0xf1,0x1a)
	{	
		*ppv=(LPVOID)this;
		this->AddRef();
		return S_OK;
	}

	/* IID_IDirect3DSurface9 */
	/* {0CFBAF3A-9FF6-429a-99B3-A2796AF8B89B} */
	IF_GUID(riid,0x0cfbaf3a,0x9ff6,0x429a,0x99,0xb3,0xa2,0x79)
		return this->GetSurfaceLevel(0,(IDirect3DSurface9**)ppv);
	
	return m_pActualTexture->QueryInterface(riid, ppv);
}

/**
* GetDevice on the underlying IDirect3DTexture9 will return the device used to create it. 
* Which is the actual device and not the wrapper. Therefore we have to keep track of the 
* wrapper device and return that instead.
* 
* Calling this method will increase the internal reference count on the IDirect3DDevice9 interface. 
* Failure to call IUnknown::Release when finished using this IDirect3DDevice9 interface results in a 
* memory leak.
*/
HRESULT WINAPI D3D9ProxyTexture::GetDevice(IDirect3DDevice9** ppDevice)
{
	#ifdef SHOW_CALLS
		OutputDebugString("called D3D9ProxyTexture::GetDevice");
	#endif
	if (!m_pOwningDevice)
		return D3DERR_INVALIDCALL;
	else {
		*ppDevice = m_pOwningDevice;
		m_pOwningDevice->AddRef();
		return D3D_OK;
	}
}

/**
* Sets private data on both (left/right) textures.
***/
HRESULT WINAPI D3D9ProxyTexture::SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	#ifdef SHOW_CALLS
		OutputDebugString("called D3D9ProxyTexture::SetPrivateData");
	#endif
	if (IsStereo() && m_pActualTextureRight)
		m_pActualTextureRight->SetPrivateData(refguid, pData, SizeOfData, Flags);

	return m_pActualTexture->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

/**
* Frees private data on both (left/right) textures.
***/
HRESULT WINAPI D3D9ProxyTexture::FreePrivateData(REFGUID refguid)
{
	#ifdef SHOW_CALLS
		OutputDebugString("called D3D9ProxyTexture::FreePrivateData");
	#endif
	if (IsStereo() && m_pActualTextureRight)
		m_pActualTextureRight->FreePrivateData(refguid);

	return m_pActualTexture->FreePrivateData(refguid);
}

/**
* Sets priority on both (left/right) textures.
***/
DWORD WINAPI D3D9ProxyTexture::SetPriority(DWORD PriorityNew)
{
	#ifdef SHOW_CALLS
		OutputDebugString("called D3D9ProxyTexture::SetPriority");
	#endif
	if (IsStereo() && m_pActualTextureRight)
		m_pActualTextureRight->SetPriority(PriorityNew);

	return m_pActualTexture->SetPriority(PriorityNew);
}

/**
* Calls method on both (left/right) textures.
***/
void WINAPI D3D9ProxyTexture::PreLoad()
{
	#ifdef SHOW_CALLS
		OutputDebugString("called D3D9ProxyTexture::PreLoad");
	#endif
	if (IsStereo() && m_pActualTextureRight)
		m_pActualTextureRight->PreLoad();

	return m_pActualTexture->PreLoad();
}

/**
* Sets LOD on both (left/right) texture.
***/
DWORD WINAPI D3D9ProxyTexture::SetLOD(DWORD LODNew)
{
	#ifdef SHOW_CALLS
		OutputDebugString("called D3D9ProxyTexture::SetLOD");
	#endif
	if (IsStereo() && m_pActualTextureRight)
		m_pActualTextureRight->SetLOD(LODNew);

	return m_pActualTexture->SetLOD(LODNew);
}

/**
* Sets filter type on both (left/right) texture.
***/
HRESULT WINAPI D3D9ProxyTexture::SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType)
{
	#ifdef SHOW_CALLS
		OutputDebugString("called D3D9ProxyTexture::SetAutoGenFilterType");
	#endif
	if (IsStereo() && m_pActualTextureRight)
		m_pActualTextureRight->SetAutoGenFilterType(FilterType);

	return m_pActualTexture->SetAutoGenFilterType(FilterType);
}

/**
* Generates sub levels on both (left/right) texture.
***/
void WINAPI D3D9ProxyTexture::GenerateMipSubLevels()
{
	#ifdef SHOW_CALLS
		OutputDebugString("called D3D9ProxyTexture::GenerateMipSubLevels");
	#endif
	if (IsStereo() && m_pActualTextureRight)
		m_pActualTextureRight->GenerateMipSubLevels();

	return m_pActualTexture->GenerateMipSubLevels();
}

/**
* If proxy surface is already stored on this level, return this one, otherwise create it.
* To create a new stored surface level, call the method on both (left/right) actual textures.
***/
HRESULT WINAPI D3D9ProxyTexture::GetSurfaceLevel(UINT Level, IDirect3DSurface9** ppSurfaceLevel)
{
	#ifdef SHOW_CALLS
		OutputDebugString("called D3D9ProxyTexture::GetSurfaceLevel");
	#endif
	HRESULT finalResult;

	// Have we already got a Proxy for this surface level?
	if (m_wrappedSurfaceLevels.count(Level) == 1) { // yes

		// TODO Should we call through to underlying texture and make sure the result of doing this operation on the 
		// underlying texture would still be a success? (not if we don't have to, will see if it becomes a problem)

		*ppSurfaceLevel = m_wrappedSurfaceLevels[Level];
		(*ppSurfaceLevel)->AddRef();

		finalResult = D3D_OK;
	}
	else {
		// Get underlying surfaces (stereo pair made from the surfaces at the same level in the left and right textues), 
		//  wrap, then store in m_wrappedSurfaceLevels and return the wrapped surface
		IDirect3DSurface9* pActualSurfaceLevelLeft = NULL;
		IDirect3DSurface9* pActualSurfaceLevelRight = NULL;

		HRESULT leftResult = m_pActualTexture->GetSurfaceLevel(Level, &pActualSurfaceLevelLeft);

		if (IsStereo() && m_pActualTextureRight) {
			HRESULT resultRight = m_pActualTextureRight->GetSurfaceLevel(Level, &pActualSurfaceLevelRight);
			assert (leftResult == resultRight);
		}


		if (SUCCEEDED(leftResult)) {

			D3D9ProxySurface* pWrappedSurfaceLevel = new D3D9ProxySurface(m_Width, m_Height, m_Format, m_lockable, pActualSurfaceLevelLeft, pActualSurfaceLevelRight, m_pOwningDevice, this, NULL, NULL);

			if(m_wrappedSurfaceLevels.insert(std::pair<ULONG, D3D9ProxySurface*>(Level, pWrappedSurfaceLevel)).second) {
				// insertion of wrapped surface level into m_wrappedSurfaceLevels succeeded
				*ppSurfaceLevel = pWrappedSurfaceLevel;
				(*ppSurfaceLevel)->AddRef();
				finalResult = D3D_OK;
			}
			else {
				// Failure to insert should not be possible. In this case we could still return the wrapped surface,
				// however, if we did and it was requested again a new wrapped instance will be returned and things would explode
				// at some point. Better to fail fast.
				OutputDebugString(__FUNCTION__);
				OutputDebugString("\n");
				OutputDebugString("Unable to store surface level.\n");
				assert(false);

				finalResult = D3DERR_INVALIDCALL;
			}
		}
		else { 
			OutputDebugString(__FUNCTION__);
			OutputDebugString("\n");
			OutputDebugString("Error fetching actual surface level.\n");
			finalResult = leftResult;
		}
	}

	OutputDebugString("exit D3D9ProxyTexture::GetSurfaceLevel");
	return finalResult;
}

/**
* Locks rectangle on both (left/right) textures.
***/
HRESULT WINAPI D3D9ProxyTexture::LockRect(UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
	#ifdef SHOW_CALLS
		OutputDebugString("called D3D9ProxyTexture::LockRect");
	#endif

	if (m_lockable)
	{
		//Can't really handle stereo for this, so just lock on the original texture
		return m_pActualTexture->LockRect(Level, pLockedRect, pRect, Flags);
	}

	if (lockedTextures.find(Level) == lockedTextures.end())
		lockedTextures[Level] = NULL;

	//Create lockable system memory surfaces
	if (pRect && !fullSurfaces[Level])
	{
		lockedRects[Level].push_back(*pRect);
	}
	else
	{
		lockedRects[Level].clear();
		fullSurfaces[Level] = true;
	}

	HRESULT hr = D3DERR_INVALIDCALL;
	IDirect3DSurface9 *pSurface = NULL;
	if (!lockedTextures[Level])
	{
		hr = m_pOwningDevice->getActual()->CreateTexture(m_Width, m_Height, 1, D3DUSAGE_RENDERTARGET, 
			m_Format, D3DPOOL_SYSTEMMEM, &lockedTextures[Level], NULL);
		if (FAILED(hr))
			return hr;
		hr = lockedTextures[Level]->GetSurfaceLevel(0, &pSurface);
		if (FAILED(hr))
			return hr;

		IDirect3DSurface9 *pActualSurface = NULL;
		hr = m_pActualTexture->GetSurfaceLevel(Level, &pActualSurface);
		if (FAILED(hr))
			return hr;
		hr = m_pOwningDevice->getActual()->GetRenderTargetData(pActualSurface, pSurface);
		if (FAILED(hr))
			return hr;

		hr = pSurface->LockRect(pLockedRect, pRect, Flags);
		if (FAILED(hr))
			return hr;
		pActualSurface->Release();
		pSurface->Release();
	}

	OutputDebugString("exit D3D9ProxySurface::LockRect");

	return hr;
}
	
/**
* Unlocks rectangle on both (left/right) textures.
***/
HRESULT WINAPI D3D9ProxyTexture::UnlockRect(UINT Level)
{
	#ifdef SHOW_CALLS
		OutputDebugString("called D3D9ProxyTexture::UnlockRect");
	#endif

	if (lockedRects[Level].size() == 0 && !fullSurfaces[Level])
		return D3DERR_INVALIDCALL;

	HRESULT hr = lockedTextures[Level] ? lockedTextures[Level]->UnlockRect(0) : D3DERR_INVALIDCALL;
	if (FAILED(hr))
		return hr;

	IDirect3DSurface9 *pSurface = NULL;
	hr = lockedTextures[Level]->GetSurfaceLevel(0, &pSurface);
	if (FAILED(hr))
		return hr;

	if (IsStereo())
	{
		IDirect3DSurface9 *pActualSurfaceRight = NULL;
		hr = m_pActualTextureRight->GetSurfaceLevel(Level, &pActualSurfaceRight);
		if (FAILED(hr))
			return hr;

		if (fullSurfaces[Level])
		{
			hr = m_pOwningDevice->getActual()->UpdateSurface(pSurface, NULL, pActualSurfaceRight, NULL);
			if (FAILED(hr))
				return hr;
		}
		else
		{
			std::vector<RECT>::iterator rectIter = lockedRects[Level].begin();
			while (rectIter != lockedRects[Level].end())
			{
				POINT p;
				p.x = rectIter->left;
				p.y = rectIter->top;
				hr = m_pOwningDevice->getActual()->UpdateSurface(pSurface, &(*rectIter), pActualSurfaceRight, &p);
				if (FAILED(hr))
					return hr;
				rectIter++;
			}
		}

		pActualSurfaceRight->Release();
	}

	IDirect3DSurface9 *pActualSurface = NULL;
	hr = m_pActualTexture->GetSurfaceLevel(Level, &pActualSurface);
	if (FAILED(hr))
		return hr;
	if (fullSurfaces[Level])
	{
		hr = m_pOwningDevice->getActual()->UpdateSurface(pSurface, NULL, pActualSurface, NULL);
		if (FAILED(hr))
			return hr;
	}
	else
	{
		std::vector<RECT>::iterator rectIter = lockedRects[Level].begin();
		while (rectIter != lockedRects[Level].end())
		{
			POINT p;
			p.x = rectIter->left;
			p.y = rectIter->top;
			hr = m_pOwningDevice->getActual()->UpdateSurface(pSurface, &(*rectIter), pActualSurface, &p);
			if (FAILED(hr))
				return hr;
			rectIter++;
		}
	}

	//Release everything
	pActualSurface->Release();
	pSurface->Release();
	lockedTextures[Level]->Release();
	lockedTextures[Level] = NULL;
	
	fullSurfaces[Level] = false;
	return hr;
}

/**
* Adds dirty rectangle on both (left/right) textures.
***/
HRESULT WINAPI D3D9ProxyTexture::AddDirtyRect(CONST RECT* pDirtyRect)
{
	#ifdef SHOW_CALLS
		OutputDebugString("called D3D9ProxyTexture::AddDirtyRect");
	#endif
	if (IsStereo() && m_pActualTextureRight)
		m_pActualTextureRight->AddDirtyRect(pDirtyRect);

	return m_pActualTexture->AddDirtyRect(pDirtyRect);
}

/**
* Returns the left texture.
***/
IDirect3DTexture9* D3D9ProxyTexture::getActualMono()
{
	#ifdef SHOW_CALLS
		OutputDebugString("called D3D9ProxyTexture::getActualMono");
	#endif
	return getActualLeft();
}

/**
* Returns the left texture.
***/
IDirect3DTexture9* D3D9ProxyTexture::getActualLeft()
{
	#ifdef SHOW_CALLS
		OutputDebugString("called D3D9ProxyTexture::getActualLeft");
	#endif
	return m_pActualTexture;
}

/**
* Returns the right texture.
***/
IDirect3DTexture9* D3D9ProxyTexture::getActualRight()
{
	#ifdef SHOW_CALLS
		OutputDebugString("called D3D9ProxyTexture::getActualRight");
	#endif
	return m_pActualTextureRight;
}

/**
* True if right texture present.
***/
bool D3D9ProxyTexture::IsStereo() 
{
	#ifdef SHOW_CALLS
		char buffer[32];
		sprintf_s(buffer, "m_pActualTexture = 0x%x", (DWORD)m_pActualTexture);
		OutputDebugString(buffer);
	#endif
	return (m_pActualTextureRight != NULL);
}
