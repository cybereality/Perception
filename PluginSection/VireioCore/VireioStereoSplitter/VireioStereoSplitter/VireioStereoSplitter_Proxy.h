/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Stereo Splitter - Vireio Perception Render Target Handler
Copyright (C) 2015 Denis Reischl

File <VireioStereoSplitter_Proxy.h>:
Copyright (C) 2017 Denis Reischl

All proxy classes directly derive from Vireio source code originally
authored by Chris Drain and Simon Brown (v1.0 - v3.0).

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
********************************************************************/

#include <d3d9.h>
#include <unordered_map>
#include <mutex>

#pragma region defines/types
#define IF_GUID(riid,a,b,c,d,e,f,g) if ((riid.Data1==a)&&(riid.Data2==b)&&(riid.Data3==c)&&(riid.Data4[0]==d)&&(riid.Data4[1]==e)&&(riid.Data4[2]==f)&&(riid.Data4[3]==g))
#define SHOW_CALL(name) // OutputDebugStringA(name) // CallLogger call(name)
#define SAFE_RELEASE(a) if (a) { a->Release(); a = nullptr; }
inline void _assert(const char* expression, const char* file, int line)
{
	fprintf(stderr, "Assertion '%s' failed, file '%s' line '%d'.", expression, file, line);
	abort();
}
#ifdef _DEBUG
#define assert(EXPRESSION) ((void)0)
#else
#define assert(EXPRESSION) ((EXPRESSION) ? (void)0 : _assert(#EXPRESSION, __FILE__, __LINE__))
#endif

/**
* Pair to use as key for storing surface levels.
***/
typedef std::pair<D3DCUBEMAP_FACES, UINT> CubeSurfaceKey;

/**
* Hash for surface level key.
***/
struct hash_CubeSurfaceKey
{
	size_t operator()(const CubeSurfaceKey &cubeSurfaceKey) const
	{
		return std::hash<int>()(cubeSurfaceKey.first) ^ std::hash<UINT>()(cubeSurfaceKey.second);
	}
};

/**
* Must be set to TRUE whenever the device is in use.
***/
static bool s_bDeviceInUseByProxy = false;
#pragma endregion

/**
*  Direct 3D stereo surface class.
*  Overwrites IDirect3DSurface9 and imbeds the actual surfaces left/right.
*/
class IDirect3DStereoSurface9 : public IDirect3DSurface9
{
public:
	/**
	* Constructor.
	* @param pcActualSurface Imbed actual surface.
	***/
	IDirect3DStereoSurface9(IDirect3DSurface9* pcActualSurfaceLeft, IDirect3DSurface9* pcActualSurfaceRight,
		IDirect3DDevice9* pcOwningDevice, IUnknown* pcWrappedContainer, HANDLE pSharedHandleLeft, HANDLE pSharedHandleRight) :
		m_pcActualSurface(pcActualSurfaceLeft),
		m_pcActualSurfaceRight(pcActualSurfaceRight),
		m_pcOwningDevice(pcOwningDevice),
		m_pcWrappedContainer(pcWrappedContainer),
		m_SharedHandleLeft(pSharedHandleLeft),
		m_SharedHandleRight(pSharedHandleRight),
		lockableSysMemTexture(NULL),
		fullSurface(false)
	{
		SHOW_CALL("D3D9ProxySurface::D3D9ProxySurface()");

		assert(pcOwningDevice != NULL);

		if (!pcActualSurfaceLeft)
		{
			m_unRefCount = 1;
			OutputDebugString(L"[STS] Trying to create zero left actual surface.");
		}
		else
		{
			// set ref count by actual ref count
			pcActualSurfaceLeft->AddRef();
			m_unRefCount = pcActualSurfaceLeft->Release();
		}
	}

	/**
	* Destructor.
	* Releases embedded surfaces + wrapper.
	***/
	~IDirect3DStereoSurface9()
	{
		SHOW_CALL("D3D9ProxySurface::~D3D9ProxySurface()");
		SAFE_RELEASE(lockableSysMemTexture);
		SAFE_RELEASE(m_pcActualSurfaceRight);
		SAFE_RELEASE(m_pcActualSurface);
	}

	/**
	* Base QueryInterface functionality.
	***/
	HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv)
	{
		SHOW_CALL("D3D9ProxySurface::QueryInterface()");

		return m_pcActualSurface->QueryInterface(riid, ppv);
	}

	/**
	* Behaviour determined through observing D3D with various test cases.
	*
	* Creating a surface should only increase the device ref count iff the surface has no parent container.
	* (The container adds one ref to the device for it and all its surfaces)

	* If a surface has a container then adding references to the surface should increase the ref count on
	* the container instead of the surface. The surface shares a total ref count with the container, when
	* it reaches 0 the container and its surfaces are destroyed. This is handled by sending all Add/Release
	* on to the container when there is one.
	***/
	ULONG WINAPI AddRef()
	{
		SHOW_CALL("D3D9ProxySurface::AddRef()");

		// if surface is in a container increase count on container instead of the surface
		if (m_pcWrappedContainer)
		{
			return m_pcWrappedContainer->AddRef();
		}
		else
		{
			// otherwise track references normally
			return ++m_unRefCount;
		}
	}

	/**
	* Releases wrapped container if present else the base surface.
	***/
	ULONG WINAPI Release()
	{
		SHOW_CALL("D3D9ProxySurface::Release()");
		if (m_pcWrappedContainer)
		{
			return m_pcWrappedContainer->Release();
		}
		else
		{
			if (--m_unRefCount == 0)
			{
				delete this;
				return 0;
			}

			return m_unRefCount;
		}
	}

	/**
	* Base GetDevice functionality.
	***/
	HRESULT WINAPI GetDevice(IDirect3DDevice9** ppcDevice)
	{
		SHOW_CALL("D3D9ProxySurface::GetDevice()");
		return m_pcActualSurface->GetDevice(ppcDevice);
	}

	/**
	* Sets private data on both (left/right) surfaces.
	***/
	HRESULT WINAPI SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
	{
		SHOW_CALL("D3D9ProxySurface::SetPrivateData");
		if (IsStereo())
			m_pcActualSurfaceRight->SetPrivateData(refguid, pData, SizeOfData, Flags);

		return m_pcActualSurface->SetPrivateData(refguid, pData, SizeOfData, Flags);
	}

	/**
	* Base GetPrivateData functionality.
	***/
	HRESULT WINAPI GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
	{
		SHOW_CALL("D3D9ProxySurface::GetPrivateData()");
		return m_pcActualSurface->GetPrivateData(refguid, pData, pSizeOfData);
	}

	/**
	* Frees private data on both (left/right) surfaces.
	***/
	HRESULT WINAPI FreePrivateData(REFGUID refguid)
	{
		SHOW_CALL("D3D9ProxySurface::FreePrivateData");
		if (IsStereo())
			m_pcActualSurfaceRight->FreePrivateData(refguid);

		return m_pcActualSurface->FreePrivateData(refguid);
	}

	/**
	* Sets priority on both (left/right) surfaces.
	***/
	DWORD WINAPI SetPriority(DWORD PriorityNew)
	{
		SHOW_CALL("D3D9ProxySurface::SetPriority");
		if (IsStereo())
			m_pcActualSurfaceRight->SetPriority(PriorityNew);

		return m_pcActualSurface->SetPriority(PriorityNew);
	}

	/**
	* Base GetPriority functionality.
	***/
	DWORD WINAPI GetPriority()
	{
		SHOW_CALL("D3D9ProxySurface::GetPriority()");
		return m_pcActualSurface->GetPriority();
	}

	/**
	* Calls method on both (left/right) surfaces.
	***/
	void WINAPI PreLoad()
	{
		SHOW_CALL("D3D9ProxySurface::PreLoad");
		if (IsStereo())
			m_pcActualSurfaceRight->PreLoad();

		return m_pcActualSurface->PreLoad();
	}

	/**
	* Base GetType functionality.
	***/
	D3DRESOURCETYPE WINAPI GetType()
	{
		SHOW_CALL("D3D9ProxySurface::GetType()");
		return m_pcActualSurface->GetType();
	}

	/**
	* Provides acces to parent object.
	* "Provides access to the parent cube texture or texture (mipmap) object, if this surface is a child
	* level of a cube texture or a mipmap. This method can also provide access to the parent swap chain
	* if the surface is a back-buffer child."
	*
	* "If the surface is created using CreateRenderTarget or CreateOffscreenPlainSurface or
	* CreateDepthStencilSurface, the surface is considered stand alone. In this case, GetContainer
	* will return the Direct3D device used to create the surface."
	* <http://msdn.microsoft.com/en-us/library/windows/desktop/bb205893%28v=vs.85%29.aspx>
	*
	* If the call succeeds, the reference count of the container is increased by one.
	* @return Owning device if no wrapped container present, otherwise the container.
	***/
	HRESULT WINAPI GetContainer(REFIID riid, LPVOID* ppContainer)
	{
		SHOW_CALL("D3D9ProxySurface::GetContainer");
		if (!m_pcWrappedContainer)
		{
			return m_pcActualSurface->GetContainer(riid, ppContainer);
		}

		void *pContainer = NULL;
		HRESULT queryResult = m_pcWrappedContainer->QueryInterface(riid, &pContainer);

		if (queryResult == S_OK)
		{
			*ppContainer = m_pcWrappedContainer;
			m_pcWrappedContainer->AddRef();

			return D3D_OK;
		}
		else if (queryResult == E_NOINTERFACE)
		{

			return E_NOINTERFACE;
		}
		else
		{
			return D3DERR_INVALIDCALL;
		}

		// Like GetDevice we need to return the wrapper rather than the underlying container 
		//return m_pActualSurface->GetContainer(riid, ppContainer);
	}

	/**
	* Base GetDesc functionality.
	***/
	HRESULT WINAPI GetDesc(D3DSURFACE_DESC *pDesc)
	{
		SHOW_CALL("D3D9ProxySurface::GetDesc()");

		return m_pcActualSurface->GetDesc(pDesc);
	}

	/**
	* Locks rectangle on both (left/right) surfaces.
	***/
	HRESULT WINAPI LockRect(D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
	{
		SHOW_CALL("D3D9ProxySurface::LockRect");

		D3DSURFACE_DESC desc;
		m_pcActualSurface->GetDesc(&desc);
		if (desc.Pool != D3DPOOL_DEFAULT)
		{
			// can't really handle stereo for this, so just lock on the original texture
			return m_pcActualSurface->LockRect(pLockedRect, pRect, Flags);
		}

		// guard against multithreaded access as this could be causing us problems
		std::lock_guard<std::mutex> lck(m_mtx);

		// create lockable system memory surfaces
		if (pRect && !fullSurface)
		{
			lockedRects.push_back(*pRect);
		}
		else
		{
			lockedRects.clear();
			fullSurface = true;
		}

		HRESULT hr = D3DERR_INVALIDCALL;
		IDirect3DSurface9 *pSurface = NULL;
		bool createdTexture = false;
		if (!lockableSysMemTexture)
		{
			// set the static device bool when using it
			s_bDeviceInUseByProxy = true;
			hr = m_pcOwningDevice->CreateTexture(desc.Width, desc.Height, 1, 0,
				desc.Format, D3DPOOL_SYSTEMMEM, &lockableSysMemTexture, NULL);
			s_bDeviceInUseByProxy = false;

			if (FAILED(hr))
				return hr;

			createdTexture = true;
		}

		lockableSysMemTexture->GetSurfaceLevel(0, &pSurface);

		// only copy the render taget (if possible) on the creation of the memory texture
		if (createdTexture)
		{
			s_bDeviceInUseByProxy = true;
			hr = D3DXLoadSurfaceFromSurface(pSurface, NULL, NULL, m_pcActualSurface, NULL, NULL, D3DX_DEFAULT, 0);
			s_bDeviceInUseByProxy = false;
			if (FAILED(hr))
			{
#ifdef _DEBUG
				{ wchar_t buf[128]; wsprintf(buf, L"Failed: D3DXLoadSurfaceFromSurface hr = 0x%0.8x", hr); OutputDebugString(buf); }
#endif
			}
		}

		// and finally, lock the memory surface
		hr = pSurface->LockRect(pLockedRect, pRect, Flags);
		if (FAILED(hr))
			return hr;

		lockedRect = *pLockedRect;

		pSurface->Release();

		return hr;
	}

	/**
	* Unlocks rectangle on both (left/right) surfaces.
	***/
	HRESULT WINAPI UnlockRect()
	{
		SHOW_CALL("D3D9ProxySurface::UnlockRect");
		D3DSURFACE_DESC desc;
		m_pcActualSurface->GetDesc(&desc);
		if (desc.Pool != D3DPOOL_DEFAULT)
		{
			return m_pcActualSurface->UnlockRect();
		}

		//Guard against multithreaded access as this could be causing us problems
		std::lock_guard<std::mutex> lck(m_mtx);

		//This would mean nothing to do
		if (lockedRects.size() == 0 && !fullSurface)
			return S_OK;

		IDirect3DSurface9 *pSurface = NULL;
		HRESULT hr = lockableSysMemTexture ? lockableSysMemTexture->GetSurfaceLevel(0, &pSurface) : D3DERR_INVALIDCALL;
		if (FAILED(hr))
			return hr;

		hr = pSurface->UnlockRect();

		if (IsStereo())
		{
			if (fullSurface)
			{
				s_bDeviceInUseByProxy = true;
				hr = m_pcOwningDevice->UpdateSurface(pSurface, NULL, m_pcActualSurfaceRight, NULL);
				s_bDeviceInUseByProxy = false;
				if (FAILED(hr))
					WriteDesc(desc);
			}
			else
			{
				std::vector<RECT>::iterator rectIter = lockedRects.begin();
				while (rectIter != lockedRects.end())
				{
					POINT p;
					p.x = rectIter->left;
					p.y = rectIter->top;
					s_bDeviceInUseByProxy = true;
					hr = m_pcOwningDevice->UpdateSurface(pSurface, &(*rectIter), m_pcActualSurfaceRight, &p);
					s_bDeviceInUseByProxy = false;
					if (FAILED(hr))
						WriteDesc(desc);
					rectIter++;
				}
			}
		}

		if (fullSurface)
		{
			s_bDeviceInUseByProxy = true;
			hr = m_pcOwningDevice->UpdateSurface(pSurface, NULL, m_pcActualSurface, NULL);
			s_bDeviceInUseByProxy = false;
			if (FAILED(hr))
				WriteDesc(desc);
		}
		else
		{
			std::vector<RECT>::iterator rectIter = lockedRects.begin();
			while (rectIter != lockedRects.end())
			{
				POINT p;
				p.x = rectIter->left;
				p.y = rectIter->top;
				s_bDeviceInUseByProxy = true;
				hr = m_pcOwningDevice->UpdateSurface(pSurface, &(*rectIter), m_pcActualSurface, &p);
				s_bDeviceInUseByProxy = false;
				if (FAILED(hr))
					WriteDesc(desc);
				rectIter++;
			}
		}

		pSurface->Release();

		fullSurface = false;
		return hr;
	}

	/**
	* Base GetDC functionality.
	***/
	HRESULT WINAPI GetDC(HDC *phdc)
	{
		SHOW_CALL("D3D9ProxySurface::GetDC()");

		return m_pcActualSurface->GetDC(phdc);
	}

	/**
	* Releases device context on both (left/right) surfaces.
	***/
	HRESULT WINAPI ReleaseDC(HDC hdc)
	{
		SHOW_CALL("D3D9ProxySurface::ReleaseDC");
		if (IsStereo())
			m_pcActualSurfaceRight->ReleaseDC(hdc);

		return m_pcActualSurface->ReleaseDC(hdc);
	}

	/*** IDirect3DStereoSurface9 methods ***/
	IDirect3DSurface9* GetActualMono() { return GetActualLeft(); }
	IDirect3DSurface9* GetActualLeft() { return m_pcActualSurface; }
	IDirect3DSurface9* GetActualRight() { return m_pcActualSurfaceRight; }
	HANDLE GetHandleLeft() { return m_SharedHandleLeft; }
	HANDLE GetHandleRight() { return m_SharedHandleRight; }
	bool IsStereo(){ return m_pcActualSurfaceRight != NULL; }
	void WriteDesc(D3DSURFACE_DESC &desc)
	{
		{
			{ wchar_t buf[128]; wsprintf(buf, L"Actual Surface Format = 0x%0.8x", desc.Format); OutputDebugString(buf); }
			{ wchar_t buf[128]; wsprintf(buf, L"Actual Surface Height = 0x%0.8x", desc.Height); OutputDebugString(buf); }
			{ wchar_t buf[128]; wsprintf(buf, L"Actual Surface Width = 0x%0.8x", desc.Width); OutputDebugString(buf); }
			{ wchar_t buf[128]; wsprintf(buf, L"Actual Surface MultiSampleQuality = 0x%0.8x", desc.MultiSampleQuality); OutputDebugString(buf); }
			{ wchar_t buf[128]; wsprintf(buf, L"Actual Surface MultiSampleType = 0x%0.8x", desc.MultiSampleType); OutputDebugString(buf); }
			{ wchar_t buf[128]; wsprintf(buf, L"Actual Surface Pool = 0x%0.8x", desc.Pool); OutputDebugString(buf); }
			{ wchar_t buf[128]; wsprintf(buf, L"Actual Surface Type = 0x%0.8x", desc.Type); OutputDebugString(buf); }
			{ wchar_t buf[128]; wsprintf(buf, L"Actual Surface Usage = 0x%0.8x", desc.Usage); OutputDebugString(buf); }
		}
	}

protected:
	/**
	* The actual surface.
	***/
	IDirect3DSurface9* m_pcActualSurface;
	/**
	* The actual right surface embedded.
	* NULL for surfaces that aren't being duplicated.
	***/
	IDirect3DSurface9* m_pcActualSurfaceRight;
	/**
	* Device that created this surface.
	* If in a container we don't add a ref or release the device when done because...
	* D3D only keeps one ref to the device for the container and all its surfaces. A contained
	* Surface doesn't exist without its container, if the container is destroyed so is the surface.
	* The container handles the device ref count changes.
	*
	* This device pointer is maintained here to simplify GetDevice. Normally keeping the pointer would need
	* an add ref, but that would produce different results compared to D3Ds normal behvaiour.
	* As the surface is destroyed with the container this should be safe enough.
	*
	* It would probably be more correct to remove this pointer and use QueryInterface to check for
	* IDirect3DResource9 or Direct3DISwapChain9 then cast the container to the appropriate interface
	* and the use GetDevice to fetch the device.
	***/
	IDirect3DDevice9* m_pcOwningDevice;
	/**
	* Internal reference counter.
	***/
	ULONG m_unRefCount;
	/**
	* Container this surface is part of. Texture, CubeTexture, SwapChain, (other?) NULL if standalone surface.
	*
	* If a surface is in a container then they use the containers ref count as a shared total ref count.
	* The container is responsible for deleting any surfaces it contains when the ref count reaches 0.
	* Surfaces must be deleted before the container.
	***/
	IUnknown* const m_pcWrappedContainer;

	/**
	* Handles for shared access
	*/
	HANDLE m_SharedHandleLeft;
	HANDLE m_SharedHandleRight;

	// Special handling required for locking rectangles if we are using Dx9Ex
	std::mutex m_mtx;
	std::vector<RECT> lockedRects;
	bool fullSurface;
	IDirect3DTexture9* lockableSysMemTexture;
	D3DLOCKED_RECT lockedRect;
};

/**
*  Direct 3D proxy base texture class.
*  Overwrites IDirect3DBaseTexture9 and imbeds the additional right texture pointer.
*/
class IDirect3DStereoBaseTexture9 : public IDirect3DBaseTexture9
{
public:
	/**
	* Constructor.
	* @param pcActualTexture Imbed actual texture.
	***/
	IDirect3DStereoBaseTexture9(IDirect3DBaseTexture9* pcActualTextureLeft, IDirect3DBaseTexture9* pcActualTextureRight, IDirect3DDevice9* pcOwningDevice) :
		m_pcActualTexture(pcActualTextureLeft),
		m_pcActualTextureRight(pcActualTextureRight),
		m_pcOwningDevice(pcOwningDevice)
	{
		SHOW_CALL("D3D9ProxyTexture::D3D9ProxyTexture");
		assert(pcOwningDevice != NULL);

		if (!pcActualTextureLeft)
		{
			OutputDebugString(L"[STS] Trying to create a zero left texture. \n");
			m_unRefCount = 1;
		}
		else
		{
			// set ref count by actual ref count
			pcActualTextureLeft->AddRef();
			m_unRefCount = pcActualTextureLeft->Release();
		}
	}
	/**
	* Destructor.
	* Releases embedded texture.
	***/
	virtual ~IDirect3DStereoBaseTexture9()
	{
		SHOW_CALL("D3D9ProxyTexture::~D3D9ProxyTexture");

		SAFE_RELEASE(m_pcActualTexture);
		SAFE_RELEASE(m_pcActualTextureRight);
	}

	/*** IUnknown methods ***/

	/**
	* Base QueryInterface method.
	***/
	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv)
	{
		SHOW_CALL("IDirect3DStereoBaseTexture9::QueryInterface");
		return m_pcActualTexture->QueryInterface(riid, ppv);
	}

	/**
	* Base AddRef functionality.
	***/
	virtual ULONG WINAPI AddRef()
	{
		SHOW_CALL("IDirect3DStereoBaseTexture9::AddRef()");
		return ++m_unRefCount;
	}

	/**
	* Base Release functionality.
	***/
	virtual ULONG WINAPI Release()
	{
		SHOW_CALL("IDirect3DStereoBaseTexture9::Release()");
		if (--m_unRefCount == 0)
		{
			delete this;
			return 0;
		}
		return m_unRefCount;
	}

	/**
	* Base GetDevice functionality.
	***/
	HRESULT WINAPI GetDevice(IDirect3DDevice9** ppcDevice)
	{
		SHOW_CALL("IDirect3DStereoBaseTexture9::GetDevice()");
		if (!m_pcOwningDevice)
			return D3DERR_INVALIDCALL;
		else
		{
			*ppcDevice = m_pcOwningDevice;
			s_bDeviceInUseByProxy = true;
			m_pcOwningDevice->AddRef();
			s_bDeviceInUseByProxy = false;
			return D3D_OK;
		}
	}

	/**
	* Base SetPrivateData functionality.
	***/
	HRESULT WINAPI SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
	{
		SHOW_CALL("IDirect3DStereoBaseTexture9::SetPrivateData");
		if (m_pcActualTextureRight)
			m_pcActualTextureRight->SetPrivateData(refguid, pData, SizeOfData, Flags);

		return m_pcActualTexture->SetPrivateData(refguid, pData, SizeOfData, Flags);
	}

	/**
	* Base GetPrivateData functionality.
	***/
	HRESULT WINAPI GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
	{
		SHOW_CALL("IDirect3DStereoBaseTexture9::GetPrivateData()");
		return m_pcActualTexture->GetPrivateData(refguid, pData, pSizeOfData);
	}

	/**
	* FreePrivateData left/right.
	***/
	HRESULT WINAPI FreePrivateData(REFGUID refguid)
	{
		SHOW_CALL("IDirect3DStereoBaseTexture9::FreePrivateData");
		if (m_pcActualTextureRight)
			m_pcActualTextureRight->FreePrivateData(refguid);

		return m_pcActualTexture->FreePrivateData(refguid);
	}

	/**
	* Sets priority on both (left/right) textures.
	***/
	DWORD WINAPI SetPriority(DWORD PriorityNew)
	{
		SHOW_CALL("IDirect3DStereoBaseTexture9::SetPriority");
		if (m_pcActualTextureRight)
			m_pcActualTextureRight->SetPriority(PriorityNew);

		return m_pcActualTexture->SetPriority(PriorityNew);
	}

	/**
	* Base GetPriority functionality.
	***/
	DWORD WINAPI GetPriority()
	{
		SHOW_CALL("IDirect3DStereoBaseTexture9::GetPriority()");
		return m_pcActualTexture->GetPriority();
	}

	/**
	* Calls method on both (left/right) textures.
	***/
	void WINAPI PreLoad()
	{
		SHOW_CALL("IDirect3DStereoBaseTexture9::PreLoad");
		if (m_pcActualTextureRight)
			m_pcActualTextureRight->PreLoad();

		return m_pcActualTexture->PreLoad();
	}

	/**
	* Base GetType functionality.
	***/
	D3DRESOURCETYPE WINAPI GetType()
	{
		SHOW_CALL("IDirect3DStereoBaseTexture9::GetType()");
		return m_pcActualTexture->GetType();
	}

	/**
	* Base SetLOD functionality.
	***/
	DWORD WINAPI SetLOD(DWORD LODNew)
	{
		SHOW_CALL("IDirect3DStereoBaseTexture9::SetLOD");
		if (m_pcActualTextureRight)
			m_pcActualTextureRight->SetLOD(LODNew);

		return m_pcActualTexture->SetLOD(LODNew);
	}

	/**
	* Base GetLOD functionality.
	***/
	DWORD WINAPI GetLOD()
	{
		SHOW_CALL("IDirect3DStereoBaseTexture9::GetLOD()");
		return m_pcActualTexture->GetLOD();
	}

	/**
	* Base GetLevelCount functionality.
	***/
	DWORD WINAPI GetLevelCount()
	{
		SHOW_CALL("IDirect3DStereoBaseTexture9::GetLevelCount()");
		return m_pcActualTexture->GetLevelCount();
	}

	/**
	* Base SetAutoGenFilterType functionality.
	***/
	HRESULT WINAPI SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType)
	{
		SHOW_CALL("IDirect3DStereoBaseTexture9::SetAutoGenFilterType");
		if (m_pcActualTextureRight)
			m_pcActualTextureRight->SetAutoGenFilterType(FilterType);

		return m_pcActualTexture->SetAutoGenFilterType(FilterType);
	}

	/**
	* Base GetAutoGenFilterType functionality.
	***/
	D3DTEXTUREFILTERTYPE WINAPI GetAutoGenFilterType()
	{
		SHOW_CALL("IDirect3DStereoBaseTexture9::GetAutoGenFilterType");
		return m_pcActualTexture->GetAutoGenFilterType();
	}

	/**
	* Base GenerateMipSubLevels functionality.
	***/
	void WINAPI GenerateMipSubLevels()
	{
		SHOW_CALL("IDirect3DStereoBaseTexture9::GenerateMipSubLevels");
		if (m_pcActualTextureRight)
			m_pcActualTextureRight->GenerateMipSubLevels();

		m_pcActualTexture->GenerateMipSubLevels();
	}

protected:
	/**
	* The actual texture.
	***/
	union
	{
		IDirect3DBaseTexture9* m_pcActualTexture;
		IDirect3DTexture9* m_pcActualTextureT;
		IDirect3DCubeTexture9* m_pcActualTextureC;
		IDirect3DVolumeTexture9* m_pcActualTextureV;
	};
	/**
	* The actual right texture embedded.
	***/
	union
	{
		IDirect3DBaseTexture9* m_pcActualTextureRight;
		IDirect3DTexture9* m_pcActualTextureRightT;
		IDirect3DCubeTexture9* m_pcActualTextureRightC;
		IDirect3DVolumeTexture9* m_pcActualTextureRightV;
	};
	/**
	* Pointer to the owning D3D device.
	***/
	IDirect3DDevice9* m_pcOwningDevice;
	/**
	* Internal reference counter.
	***/
	ULONG m_unRefCount;
};

/**
*  Direct 3D proxy texture class.
*  Overwrites IDirect3DTexture9 and imbeds the additional right texture pointer.
*/
class IDirect3DStereoTexture9 : public IDirect3DTexture9, public IDirect3DStereoBaseTexture9
{
public:
	/**
	* Constructor.
	* @param pcActualTexture Imbed actual texture.
	***/
	IDirect3DStereoTexture9(IDirect3DTexture9* pcActualTextureLeft, IDirect3DTexture9* pcActualTextureRight, IDirect3DDevice9* pcOwningDevice) :
		IDirect3DStereoBaseTexture9(pcActualTextureLeft, pcActualTextureRight, pcOwningDevice),
		m_aaWrappedSurfaceLevels(),
		lockableSysMemTexture(NULL)
	{
	}

	/**
	* Destructor.
	* Releases embedded texture.
	***/
	virtual ~IDirect3DStereoTexture9()
	{
		SHOW_CALL("D3D9ProxyTexture::~D3D9ProxyTexture");

		// delete all surfaces in m_levels
		auto it = m_aaWrappedSurfaceLevels.begin();
		while (it != m_aaWrappedSurfaceLevels.end())
		{
			// we have to explicitly delete the Surfaces here as the Release behaviour of the surface would get stuck in a loop
			// calling back to the container Release.
			delete it->second;
			it = m_aaWrappedSurfaceLevels.erase(it);
		}

		// cleanup lockable textures
		auto it2 = lockableSysMemTexture.begin();
		while (it2 != lockableSysMemTexture.end())
		{
			if (it2->second) it2->second->Release();
			++it2;
		}

		// call base desctructor
		IDirect3DStereoBaseTexture9::~IDirect3DStereoBaseTexture9();
	}

	/*** IUnknown methods ***/

	/**
	* Ensures Skyrim works (and maybe other games).
	* Skyrim sometimes calls QueryInterface() to get the underlying surface instead of GetSurfaceLevel().
	* It even calls it to query the texture class.
	* @return (this) in case of query texture interface, GetSurfaceLevel() in case of query surface.
	***/
	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv)
	{
		SHOW_CALL("D3D9ProxyTexture::QueryInterface");

		/* IID_IDirect3DTexture9 */
		/* {85C31227-3DE5-4f00-9B3A-F11AC38C18B5} */
		IF_GUID(riid, 0x85c31227, 0x3de5, 0x4f00, 0x9b, 0x3a, 0xf1, 0x1a)
		{
			*ppv = (LPVOID)this;
			IDirect3DStereoBaseTexture9::AddRef();
			return S_OK;
		}

		/* IID_IDirect3DBaseTexture9 */
		/* {580CA87E-1D3C-4d54-991D-B7D3E3C298CE} */
		IF_GUID(riid, 0x580ca87e, 0x1d3c, 0x4d54, 0x99, 0x1d, 0xb7, 0xd3)//, 0xe3, 0xc2, 0x98, 0xce)
		{
			*ppv = (LPVOID)this;
			IDirect3DStereoBaseTexture9::AddRef();
			return S_OK;
		}

		/* IID_IDirect3DSurface9 */
		/* {0CFBAF3A-9FF6-429a-99B3-A2796AF8B89B} */
		IF_GUID(riid, 0x0cfbaf3a, 0x9ff6, 0x429a, 0x99, 0xb3, 0xa2, 0x79)
			return this->GetSurfaceLevel(0, (IDirect3DSurface9**)ppv);

		return m_pcActualTexture->QueryInterface(riid, ppv);
	}

	virtual ULONG WINAPI AddRef() { return IDirect3DStereoBaseTexture9::AddRef(); }
	virtual ULONG WINAPI Release() { return IDirect3DStereoBaseTexture9::Release(); }

	/*** IDirect3DBaseTexture9 methods	***/

	HRESULT WINAPI GetDevice(IDirect3DDevice9** ppcDevice) { return IDirect3DStereoBaseTexture9::GetDevice(ppcDevice); }
	HRESULT WINAPI SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags) { return IDirect3DStereoBaseTexture9::SetPrivateData(refguid, pData, SizeOfData, Flags); }
	HRESULT WINAPI GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData) { return IDirect3DStereoBaseTexture9::GetPrivateData(refguid, pData, pSizeOfData); }
	HRESULT WINAPI FreePrivateData(REFGUID refguid) { return IDirect3DStereoBaseTexture9::FreePrivateData(refguid); }
	DWORD WINAPI SetPriority(DWORD PriorityNew) { return IDirect3DStereoBaseTexture9::SetPriority(PriorityNew); }
	DWORD WINAPI GetPriority() { return IDirect3DStereoBaseTexture9::GetPriority(); }
	void WINAPI PreLoad() { IDirect3DStereoBaseTexture9::PreLoad(); }
	D3DRESOURCETYPE WINAPI GetType() { return IDirect3DStereoBaseTexture9::GetType(); }
	DWORD WINAPI SetLOD(DWORD LODNew) { return IDirect3DStereoBaseTexture9::SetLOD(LODNew); }
	DWORD WINAPI GetLOD() { return IDirect3DStereoBaseTexture9::GetLOD(); }
	DWORD WINAPI GetLevelCount() { return IDirect3DStereoBaseTexture9::GetLevelCount(); }
	HRESULT WINAPI SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType) { return IDirect3DStereoBaseTexture9::SetAutoGenFilterType(FilterType); }
	D3DTEXTUREFILTERTYPE WINAPI GetAutoGenFilterType() { return IDirect3DStereoBaseTexture9::GetAutoGenFilterType(); }
	void WINAPI GenerateMipSubLevels() { IDirect3DStereoBaseTexture9::GenerateMipSubLevels(); }

	/**
	* Base GetLevelDesc functionality.
	***/
	HRESULT WINAPI GetLevelDesc(UINT Level, D3DSURFACE_DESC *pDesc)
	{
		SHOW_CALL("D3D9ProxyTexture::GetLevelDesc");
		return m_pcActualTextureT->GetLevelDesc(Level, pDesc);
	}

	/**
	* If proxy surface is already stored on this level, return this one, otherwise create it.
	* To create a new stored surface level, call the method on both (left/right) actual textures.
	***/
	HRESULT WINAPI GetSurfaceLevel(UINT Level, IDirect3DSurface9** ppSurfaceLevel)
	{
		SHOW_CALL("D3D9ProxyTexture::GetSurfaceLevel");

		HRESULT finalResult;

		// Have we already got a Proxy for this surface level ??
		if (m_aaWrappedSurfaceLevels.count(Level) == 1)
		{
			// yes !!

			// TODO Should we call through to underlying texture and make sure the result of doing this operation on the 
			// underlying texture would still be a success? (not if we don't have to, will see if it becomes a problem)

			*ppSurfaceLevel = m_aaWrappedSurfaceLevels[Level];
			(*ppSurfaceLevel)->AddRef();

			finalResult = D3D_OK;
		}
		else
		{
			// Get underlying surfaces (stereo pair made from the surfaces at the same level in the left and right textues), 
			// wrap, then store in m_wrappedSurfaceLevels and return the wrapped surface
			static IDirect3DSurface9* pcActualSurfaceLevelLeft = NULL;
			pcActualSurfaceLevelLeft = NULL;
			static IDirect3DSurface9* pcActualSurfaceLevelRight = NULL;
			pcActualSurfaceLevelRight = NULL;

			HRESULT leftResult = m_pcActualTextureT->GetSurfaceLevel(Level, &pcActualSurfaceLevelLeft);

			if (IsStereo() && m_pcActualTextureRight)
			{
				HRESULT resultRight = m_pcActualTextureRightT->GetSurfaceLevel(Level, &pcActualSurfaceLevelRight);
				assert(leftResult == resultRight);
			}

			if (SUCCEEDED(leftResult))
			{
				static IDirect3DStereoSurface9* pcWrappedSurfaceLevel = NULL;
				pcWrappedSurfaceLevel = new IDirect3DStereoSurface9(pcActualSurfaceLevelLeft, pcActualSurfaceLevelRight, m_pcOwningDevice, (IDirect3DStereoBaseTexture9*)this, NULL, NULL);

				if (m_aaWrappedSurfaceLevels.insert(std::pair<ULONG, IDirect3DStereoSurface9*>(Level, pcWrappedSurfaceLevel)).second)
				{
					// insertion of wrapped surface level into m_wrappedSurfaceLevels succeeded
					*ppSurfaceLevel = pcWrappedSurfaceLevel;
					(*ppSurfaceLevel)->AddRef();

					finalResult = D3D_OK;
				}
				else
				{
					// Failure to insert should not be possible. In this case we could still return the wrapped surface,
					// however, if we did and it was requested again a new wrapped instance will be returned and things would explode
					// at some point. Better to fail fast.
					OutputDebugStringA(__FUNCTION__);
					OutputDebugString(L"\n");
					OutputDebugString(L"[STS] Unable to store surface level.\n");
					assert(false);

					finalResult = D3DERR_INVALIDCALL;
				}
			}
			else
			{
				OutputDebugStringA(__FUNCTION__);
				OutputDebugString(L"\n");
				OutputDebugString(L"[STS] Error fetching actual surface level.\n");
				finalResult = leftResult;
			}
		}
		return finalResult;
	}

	/**
	* Locks rectangle on both (left/right) textures.
	***/
	HRESULT WINAPI LockRect(UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
	{
		SHOW_CALL("D3D9ProxyTexture::LockRect");

		static D3DSURFACE_DESC desc;
		ZeroMemory(&desc, sizeof(D3DSURFACE_DESC));
		m_pcActualTextureT->GetLevelDesc(Level, &desc);
		if (desc.Pool != D3DPOOL_DEFAULT)
		{
			//Can't really handle stereo for this, so just lock on the original texture
			return m_pcActualTextureT->LockRect(Level, pLockedRect, pRect, Flags);
		}

		//Guard against multithreaded access as this could be causing us problems
		std::lock_guard<std::mutex> lck(m_mtx);

		//Initialise
		if (lockableSysMemTexture.find(Level) == lockableSysMemTexture.end())
		{
			lockableSysMemTexture[Level] = NULL;
			fullSurfaces[Level] = false;
		}

		if (!pRect)
			fullSurfaces[Level] = true;
		else if (!fullSurfaces[Level])
		{
			lockedRects[Level].push_back(*pRect);
		}

		bool newTexture = false;
		HRESULT hr = D3DERR_INVALIDCALL;
		if (!lockableSysMemTexture[Level])
		{
			s_bDeviceInUseByProxy = true;
			hr = m_pcOwningDevice->CreateTexture(desc.Width, desc.Height, 1, 0,
				desc.Format, D3DPOOL_SYSTEMMEM, &lockableSysMemTexture[Level], NULL);
			s_bDeviceInUseByProxy = false;
			newTexture = true;

			if (FAILED(hr))
			{
				//DXT textures can't be less than 4 pixels in either width or height
				if (desc.Width < 4) desc.Width = 4;
				if (desc.Height < 4) desc.Height = 4;
				s_bDeviceInUseByProxy = true;
				hr = m_pcOwningDevice->CreateTexture(desc.Width, desc.Height, 1, 0,
					desc.Format, D3DPOOL_SYSTEMMEM, &lockableSysMemTexture[Level], NULL);
				s_bDeviceInUseByProxy = false;
				newTexture = true;
				if (FAILED(hr))
				{
					{ wchar_t buf[128]; wsprintf(buf, L"[STS] Failed: m_pcOwningDevice->CreateTexture hr = 0x % 0.8x", hr); OutputDebugString(buf); }
					return hr;
				}

			}
		}


		IDirect3DSurface9 *pSurface = NULL;
		hr = lockableSysMemTexture[Level]->GetSurfaceLevel(0, &pSurface);
		if (FAILED(hr))
		{
			{ wchar_t buf[128]; wsprintf(buf, L"[STS] Failed: lockableSysMemTexture[Level]->GetSurfaceLevel hr = 0x%0.8x", hr); OutputDebugString(buf); }
			return hr;
		}

		if (newTexture)
		{
			IDirect3DSurface9 *pActualSurface = NULL;
			hr = m_pcActualTextureT->GetSurfaceLevel(Level, &pActualSurface);
			if (FAILED(hr))
			{
				{ wchar_t buf[128]; wsprintf(buf, L"[STS] Failed: m_pcActualTexture->GetSurfaceLevel hr = 0x%0.8x", hr); OutputDebugString(buf); }
				return hr;
			}

			s_bDeviceInUseByProxy = true;
			hr = D3DXLoadSurfaceFromSurface(pSurface, NULL, NULL, pActualSurface, NULL, NULL, D3DX_DEFAULT, 0);
			s_bDeviceInUseByProxy = false;
			if (FAILED(hr))
			{
#ifdef _DEBUG
				{ wchar_t buf[128]; wsprintf(buf, L"[STS] Failed: D3DXLoadSurfaceFromSurface hr = 0x%0.8x", hr); OutputDebugString(buf); }
#endif
			}
			pActualSurface->Release();

			//Not a new level any more
			newSurface[Level] = false;
		}

		if (((Flags | D3DLOCK_NO_DIRTY_UPDATE) != D3DLOCK_NO_DIRTY_UPDATE) &&
			((Flags | D3DLOCK_READONLY) != D3DLOCK_READONLY))
		{
			hr = m_pcActualTextureT->AddDirtyRect(pRect);
			if (FAILED(hr))
			{
				{ wchar_t buf[128]; wsprintf(buf, L"[STS] Failed: m_pcActualTexture->AddDirtyRect hr = 0x%0.8x", hr); OutputDebugString(buf); }
				return hr;
			}
		}

		hr = pSurface->LockRect(pLockedRect, pRect, Flags);
		if (FAILED(hr))
		{
			{ wchar_t buf[128]; wsprintf(buf, L"[STS] Failed: pSurface->LockRect hr = 0x%0.8x", hr); OutputDebugString(buf); }
			return hr;
		}
		pSurface->Release();

		return hr;
	}

	/**
	* Base UnlockRect functionality.
	***/
	HRESULT WINAPI UnlockRect(UINT Level)
	{
		SHOW_CALL("D3D9ProxyTexture::UnlockRect");

		static D3DSURFACE_DESC desc;
		ZeroMemory(&desc, sizeof(D3DSURFACE_DESC));
		m_pcActualTextureT->GetLevelDesc(Level, &desc);
		if (desc.Pool != D3DPOOL_DEFAULT)
		{
			return m_pcActualTextureT->UnlockRect(Level);
		}

		//Guard against multithreaded access as this could be causing us problems
		std::lock_guard<std::mutex> lck(m_mtx);

		if (lockableSysMemTexture.find(Level) == lockableSysMemTexture.end())
			return S_OK;

		IDirect3DSurface9 *pSurface = NULL;
		HRESULT hr = lockableSysMemTexture[Level] ? lockableSysMemTexture[Level]->GetSurfaceLevel(0, &pSurface) : D3DERR_INVALIDCALL;
		if (FAILED(hr))
			return hr;

		pSurface->UnlockRect();

		if (IsStereo())
		{
			IDirect3DSurface9 *pActualSurfaceRight = NULL;
			hr = ((IDirect3DTexture9*)m_pcActualTextureRight)->GetSurfaceLevel(Level, &pActualSurfaceRight);
			if (FAILED(hr))
				return hr;

			if (fullSurfaces[Level])
			{
				s_bDeviceInUseByProxy = true;
				hr = m_pcOwningDevice->UpdateSurface(pSurface, NULL, pActualSurfaceRight, NULL);
				s_bDeviceInUseByProxy = false;
				if (FAILED(hr))
				{
#ifdef _DEBUG
					{ wchar_t buf[128]; wsprintf(buf, L"[STS] Failed: m_pcOwningDevice->getActual()->UpdateSurface hr = 0x%0.8x", hr); OutputDebugString(buf); }
					WriteDesc(Level, desc);
#endif
					//Just ignore the failed copy back, not much we can do
					hr = S_OK;
				}
			}
			else
			{
				std::vector<RECT>::iterator rectIter = lockedRects[Level].begin();
				while (rectIter != lockedRects[Level].end())
				{
					POINT p;
					p.x = rectIter->left;
					p.y = rectIter->top;
					s_bDeviceInUseByProxy = true;
					hr = m_pcOwningDevice->UpdateSurface(pSurface, &(*rectIter), pActualSurfaceRight, &p);
					s_bDeviceInUseByProxy = false;
					if (FAILED(hr))
					{
#ifdef _DEBUG
						{ wchar_t buf[128]; wsprintf(buf, L"[STS] Failed: m_pcOwningDevice->getActual()->UpdateSurface hr = 0x%0.8x", hr); OutputDebugString(buf); }
						WriteDesc(Level, desc);
#endif
						//Just ignore the failed copy back, not much we can do
						hr = S_OK;
					}
					rectIter++;
				}
			}

			pActualSurfaceRight->Release();
		}

		IDirect3DSurface9 *pActualSurface = NULL;
		hr = m_pcActualTextureT->GetSurfaceLevel(Level, &pActualSurface);
		if (FAILED(hr))
			return hr;
		if (fullSurfaces[Level])
		{
			s_bDeviceInUseByProxy = true;
			hr = m_pcOwningDevice->UpdateSurface(pSurface, NULL, pActualSurface, NULL);
			s_bDeviceInUseByProxy = false;
			if (FAILED(hr))
			{
#ifdef _DEBUG
				{ wchar_t buf[128]; wsprintf(buf, L"[STS] Failed: m_pcOwningDevice->getActual()->UpdateSurface hr = 0x%0.8x", hr); OutputDebugString(buf); }
				WriteDesc(Level, desc);
#endif

				//Just ignore the failed copy back, not much we can do
				hr = S_OK;
			}
		}
		else
		{
			std::vector<RECT>::iterator rectIter = lockedRects[Level].begin();
			while (rectIter != lockedRects[Level].end())
			{
				POINT p;
				p.x = rectIter->left;
				p.y = rectIter->top;
				s_bDeviceInUseByProxy = true;
				hr = m_pcOwningDevice->UpdateSurface(pSurface, &(*rectIter), pActualSurface, &p);
				s_bDeviceInUseByProxy = false;
				if (FAILED(hr))
				{
#ifdef _DEBUG
					{ wchar_t buf[128]; wsprintf(buf, L"[STS] Failed: m_pcOwningDevice->getActual()->UpdateSurface hr = 0x%0.8x", hr); OutputDebugString(buf); }
					WriteDesc(Level, desc);
#endif
					//Just ignore the failed copy back, not much we can do
					hr = S_OK;
				}
				rectIter++;
			}
		}

		//Release everything
		pActualSurface->Release();
		pSurface->Release();

		fullSurfaces[Level] = false;
		return hr;
	}

	/**
	* Adds dirty rectangle on both (left/right) textures.
	***/
	HRESULT WINAPI AddDirtyRect(CONST RECT* pDirtyRect)
	{
		SHOW_CALL("D3D9ProxyTexture::AddDirtyRect");
		if (IsStereo() && m_pcActualTextureRight)
			((IDirect3DTexture9*)m_pcActualTextureRight)->AddDirtyRect(pDirtyRect);

		return m_pcActualTextureT->AddDirtyRect(pDirtyRect);
	}

	/*** IDirect3DStereoTexture9 methods ***/
	IDirect3DTexture9* GetActualMono() { return GetActualLeft(); }
	IDirect3DTexture9* GetActualLeft() { return (IDirect3DTexture9*)m_pcActualTexture; }
	IDirect3DTexture9* GetActualRight() { return (IDirect3DTexture9*)m_pcActualTextureRight; }
	bool IsStereo() { return (m_pcActualTextureRight != NULL); }
	void WriteDesc(UINT Level, D3DSURFACE_DESC &desc)
	{
		{
			{ wchar_t buf[128]; wsprintf(buf, L"Actual Texture Level = 0x%0.8x", Level); OutputDebugString(buf); }
			{ wchar_t buf[128]; wsprintf(buf, L"Actual Texture Format = 0x%0.8x", desc.Format); OutputDebugString(buf); }
			{ wchar_t buf[128]; wsprintf(buf, L"Actual Texture Height = 0x%0.8x", desc.Height); OutputDebugString(buf); }
			{ wchar_t buf[128]; wsprintf(buf, L"Actual Texture Width = 0x%0.8x", desc.Width); OutputDebugString(buf); }
			{ wchar_t buf[128]; wsprintf(buf, L"Actual Texture MultiSampleQuality = 0x%0.8x", desc.MultiSampleQuality); OutputDebugString(buf); }
			{ wchar_t buf[128]; wsprintf(buf, L"Actual Texture MultiSampleType = 0x%0.8x", desc.MultiSampleType); OutputDebugString(buf); }
			{ wchar_t buf[128]; wsprintf(buf, L"Actual Texture Pool = 0x%0.8x", desc.Pool); OutputDebugString(buf); }
			{ wchar_t buf[128]; wsprintf(buf, L"Actual Texture Type = 0x%0.8x", desc.Type); OutputDebugString(buf); }
			{ wchar_t buf[128]; wsprintf(buf, L"Actual Texture Usage = 0x%0.8x", desc.Usage); OutputDebugString(buf); }
		}
	}

protected:
	/**
	* Wrapped Surface levels.
	***/
	std::unordered_map<UINT, IDirect3DStereoSurface9*> m_aaWrappedSurfaceLevels;

	//Special handling required for locking rectangles if we are using Dx9Ex
	std::mutex m_mtx;
	std::unordered_map<UINT, std::vector<RECT>> lockedRects;
	std::unordered_map<UINT, bool> fullSurfaces;
	std::unordered_map<UINT, bool> newSurface;
	std::unordered_map<UINT, IDirect3DTexture9*> lockableSysMemTexture;
};

/**
*  Direct 3D proxy stereo cube texture class.
*  Imbeds the actual texture cube.
*/
class IDirect3DStereoCubeTexture9 : public IDirect3DCubeTexture9, public IDirect3DStereoBaseTexture9
{
public:
	/**
	* Constructor.
	* @param pcActualTexture Imbed actual texture.
	***/
	IDirect3DStereoCubeTexture9(IDirect3DCubeTexture9* pcActualTextureLeft, IDirect3DCubeTexture9* pcActualTextureRight, IDirect3DDevice9* pcOwningDevice) :
		IDirect3DStereoBaseTexture9(pcActualTextureLeft, pcActualTextureRight, pcOwningDevice),
		m_aaWrappedSurfaceLevels(),
		lockableSysMemTexture(NULL)
	{
	}

	/**
	* Destructor.
	* Releases embedded texture.
	***/
	~IDirect3DStereoCubeTexture9()
	{
		SHOW_CALL("~D3D9ProxyCubeTexture()");

		// delete all surfaces in m_levels
		auto it = m_aaWrappedSurfaceLevels.begin();
		while (it != m_aaWrappedSurfaceLevels.end())
		{
			// we have to explicitly delete the Surfaces here as the Release behaviour of the surface would get stuck in a loop
			// calling back to the container Release.
			delete it->second;
			it = m_aaWrappedSurfaceLevels.erase(it);
		}

		// call base desctructor
		IDirect3DStereoBaseTexture9::~IDirect3DStereoBaseTexture9();
	}

	/*** IUnknown methods ***/

	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv) { return IDirect3DStereoBaseTexture9::QueryInterface(riid, ppv); }
	virtual ULONG WINAPI AddRef() { return IDirect3DStereoBaseTexture9::AddRef(); }
	virtual ULONG WINAPI Release() { return IDirect3DStereoBaseTexture9::Release(); }

	/*** IDirect3DBaseTexture9 methods	***/

	HRESULT WINAPI GetDevice(IDirect3DDevice9** ppcDevice) { return IDirect3DStereoBaseTexture9::GetDevice(ppcDevice); }
	HRESULT WINAPI SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags) { return IDirect3DStereoBaseTexture9::SetPrivateData(refguid, pData, SizeOfData, Flags); }
	HRESULT WINAPI GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData) { return IDirect3DStereoBaseTexture9::GetPrivateData(refguid, pData, pSizeOfData); }
	HRESULT WINAPI FreePrivateData(REFGUID refguid) { return IDirect3DStereoBaseTexture9::FreePrivateData(refguid); }
	DWORD WINAPI SetPriority(DWORD PriorityNew) { return IDirect3DStereoBaseTexture9::SetPriority(PriorityNew); }
	DWORD WINAPI GetPriority() { return IDirect3DStereoBaseTexture9::GetPriority(); }
	void WINAPI PreLoad() { IDirect3DStereoBaseTexture9::PreLoad(); }
	D3DRESOURCETYPE WINAPI GetType() { return IDirect3DStereoBaseTexture9::GetType(); }
	DWORD WINAPI SetLOD(DWORD LODNew) { return IDirect3DStereoBaseTexture9::SetLOD(LODNew); }
	DWORD WINAPI GetLOD() { return IDirect3DStereoBaseTexture9::GetLOD(); }
	DWORD WINAPI GetLevelCount() { return IDirect3DStereoBaseTexture9::GetLevelCount(); }
	HRESULT WINAPI SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType) { return IDirect3DStereoBaseTexture9::SetAutoGenFilterType(FilterType); }
	D3DTEXTUREFILTERTYPE WINAPI GetAutoGenFilterType() { return IDirect3DStereoBaseTexture9::GetAutoGenFilterType(); }
	void WINAPI GenerateMipSubLevels() { IDirect3DStereoBaseTexture9::GenerateMipSubLevels(); }

	/**
	* Base GetLevelDesc functionality.
	***/
	HRESULT WINAPI GetLevelDesc(UINT Level, D3DSURFACE_DESC *pDesc)
	{
		return m_pcActualTextureC->GetLevelDesc(Level, pDesc);
	}

	/**
	* If proxy surface is already stored on this level, return this one, otherwise create it.
	* To create a new stored surface level, call the method on both (left/right) actual textures.
	***/
	HRESULT WINAPI GetCubeMapSurface(D3DCUBEMAP_FACES FaceType, UINT Level, IDirect3DSurface9** ppCubeMapSurface)
	{
		SHOW_CALL("D3D9ProxyCubeTexture::GetCubeMapSurface");
		HRESULT finalResult;

		// Have we already got a Proxy for this surface level ??
		CubeSurfaceKey key = CubeSurfaceKey(FaceType, Level);
		if (m_aaWrappedSurfaceLevels.count(key) == 1)
		{
			// yes !!
			*ppCubeMapSurface = m_aaWrappedSurfaceLevels[key];
			(*ppCubeMapSurface)->AddRef();

			finalResult = D3D_OK;
		}
		else
		{
			// Get underlying surfaces (stereo pair made from the surfaces at the same level in the left and right textues), 
			// wrap, then store in m_wrappedSurfaceLevels and return the wrapped surface
			static IDirect3DSurface9* pActualSurfaceLevelLeft = NULL;
			pActualSurfaceLevelLeft = NULL;
			static IDirect3DSurface9* pActualSurfaceLevelRight = NULL;
			pActualSurfaceLevelRight = NULL;

			HRESULT leftResult = m_pcActualTextureC->GetCubeMapSurface(FaceType, Level, &pActualSurfaceLevelLeft);

			if (IsStereo())
			{
				HRESULT resultRight = m_pcActualTextureC->GetCubeMapSurface(FaceType, Level, &pActualSurfaceLevelRight);
				assert(leftResult == resultRight);
			}

			if (SUCCEEDED(leftResult))
			{

				static IDirect3DStereoSurface9* pWrappedSurfaceLevel = NULL;
				pWrappedSurfaceLevel = new IDirect3DStereoSurface9(pActualSurfaceLevelLeft, pActualSurfaceLevelRight, m_pcOwningDevice, (IDirect3DStereoBaseTexture9*)this, NULL, NULL);

				if (m_aaWrappedSurfaceLevels.insert(std::pair<CubeSurfaceKey, IDirect3DStereoSurface9*>(key, pWrappedSurfaceLevel)).second)
				{
					// insertion of wrapped surface level into m_wrappedSurfaceLevels succeeded
					*ppCubeMapSurface = pWrappedSurfaceLevel;
					(*ppCubeMapSurface)->AddRef();
					finalResult = D3D_OK;
				}
				else
				{
					// Failure to insert should not be possible. In this case we could still return the wrapped surface,
					// however, if we did and it was requested again a new wrapped instance will be returned and things would explode
					// at some point. Better to fail fast.
					OutputDebugStringA(__FUNCTION__);
					OutputDebugString(L"\n");
					OutputDebugString(L"Unable to store surface level.\n");
					assert(false);

					finalResult = D3DERR_INVALIDCALL;
				}
			}
			else
			{
				OutputDebugStringA(__FUNCTION__);
				OutputDebugString(L"\n");
				OutputDebugString(L"Error fetching actual surface level.\n");
				finalResult = leftResult;
			}
		}

		return finalResult;
	}

	/**
	* Base LockRect functionality.
	***/
	HRESULT WINAPI LockRect(D3DCUBEMAP_FACES FaceType, UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
	{
		SHOW_CALL("D3D9ProxyCubeTexture::LockRect");

		D3DSURFACE_DESC desc;
		m_pcActualTextureC->GetLevelDesc(0, &desc);
		if (desc.Pool != D3DPOOL_DEFAULT)
		{
			// Can't really handle stereo for this, so just lock on the original texture
			return m_pcActualTextureC->LockRect(FaceType, Level, pLockedRect, pRect, Flags);
		}

		// Guard against multithreaded access as this could be causing us problems
		std::lock_guard<std::mutex> lck(m_mtx);

		UINT key = (10 * Level) + (UINT)(FaceType);
		if (newSurface.find(key) == newSurface.end())
		{
			newSurface[key] = true;
		}

		HRESULT hr = D3DERR_INVALIDCALL;
		if (!lockableSysMemTexture)
		{
			s_bDeviceInUseByProxy = true;
			hr = m_pcOwningDevice->CreateCubeTexture(desc.Width,
				m_pcActualTexture->GetLevelCount(), 0,
				desc.Format, D3DPOOL_SYSTEMMEM, &lockableSysMemTexture, NULL);
			s_bDeviceInUseByProxy = false;
			if (FAILED(hr))
			{
				{ wchar_t buf[128]; wsprintf(buf, L"[STS] Failed: m_pcOwningDevice->getActual()->CreateCubeTexture hr = 0x%0.8x", hr); OutputDebugString(buf); }
				return hr;
			}
		}

		if (newSurface[key])
		{
			IDirect3DSurface9 *pActualSurface = NULL;
			hr = m_pcActualTextureC->GetCubeMapSurface(FaceType, Level, &pActualSurface);
			if (FAILED(hr))
			{
				{ wchar_t buf[128]; wsprintf(buf, L"[STS] Failed: m_pActualTexture->GetCubeMapSurface hr = 0x%0.8x", hr); OutputDebugString(buf); }
				return hr;
			}
			IDirect3DSurface9 *pSurface = NULL;
			hr = lockableSysMemTexture->GetCubeMapSurface(FaceType, Level, &pSurface);
			if (FAILED(hr))
			{
				{ wchar_t buf[128]; wsprintf(buf, L"[STS] Failed: lockableSysMemTexture->GetCubeMapSurface hr = 0x%0.8x", hr); OutputDebugString(buf); }
				return hr;
			}

			s_bDeviceInUseByProxy = true;
			hr = D3DXLoadSurfaceFromSurface(pSurface, NULL, NULL, pActualSurface, NULL, NULL, D3DX_DEFAULT, 0);
			s_bDeviceInUseByProxy = false;
			if (FAILED(hr))
			{
#ifdef _DEBUG
				{ wchar_t buf[128]; wsprintf(buf, L"[STS] Failed: D3DXLoadSurfaceFromSurface hr = 0x%0.8x", hr); OutputDebugString(buf); }
#endif
			}

			pSurface->Release();
			pActualSurface->Release();
		}

		if (((Flags | D3DLOCK_NO_DIRTY_UPDATE) != D3DLOCK_NO_DIRTY_UPDATE) &&
			((Flags | D3DLOCK_READONLY) != D3DLOCK_READONLY))
			hr = m_pcActualTextureC->AddDirtyRect(FaceType, pRect);

		hr = lockableSysMemTexture->LockRect(FaceType, Level, pLockedRect, pRect, Flags);
		if (FAILED(hr))
		{
			{ wchar_t buf[128]; wsprintf(buf, L"[STS] Failed: lockableSysMemTexture->LockRect hr = 0x%0.8x", hr); OutputDebugString(buf); }
		}

		return hr;
	}

	/**
	* Unlocks rectangle on both (left/right) textures.
	***/
	HRESULT WINAPI UnlockRect(D3DCUBEMAP_FACES FaceType, UINT Level)
	{
		SHOW_CALL("D3D9ProxyCubeTexture::UnlockRect");

		D3DSURFACE_DESC desc;
		m_pcActualTextureC->GetLevelDesc(Level, &desc);
		if (desc.Pool != D3DPOOL_DEFAULT)
		{
			return m_pcActualTextureC->UnlockRect(FaceType, Level);
		}

		//Guard against multithreaded access as this could be causing us problems
		std::lock_guard<std::mutex> lck(m_mtx);

		if (newSurface.size() == 0)
			return S_OK;

		HRESULT hr = lockableSysMemTexture->UnlockRect(FaceType, Level);
		if (FAILED(hr))
			return hr;

		if (IsStereo())
		{
			s_bDeviceInUseByProxy = true;
			hr = m_pcOwningDevice->UpdateTexture(lockableSysMemTexture, m_pcActualTextureRight);
			s_bDeviceInUseByProxy = false;
			if (FAILED(hr))
				return hr;
		}

		s_bDeviceInUseByProxy = true;
		hr = m_pcOwningDevice->UpdateTexture(lockableSysMemTexture, m_pcActualTexture);
		s_bDeviceInUseByProxy = false;
		if (FAILED(hr))
			return hr;

		return hr;
	}

	/**
	* Adds dirty rectangle on both (left/right) textures.
	***/
	HRESULT WINAPI AddDirtyRect(D3DCUBEMAP_FACES FaceType, CONST RECT* pDirtyRect)
	{
		SHOW_CALL("D3D9ProxyCubeTexture::AddDirtyRect");
		if (IsStereo())
			m_pcActualTextureC->AddDirtyRect(FaceType, pDirtyRect);

		return m_pcActualTextureC->AddDirtyRect(FaceType, pDirtyRect);
	}

	/*** IDirect3DStereoCubeTexture methods ***/
	IDirect3DCubeTexture9* GetActualMono() { return GetActualLeft(); }
	IDirect3DCubeTexture9* GetActualLeft() { return (IDirect3DCubeTexture9*)m_pcActualTexture; }
	IDirect3DCubeTexture9* GetActualRight() { return (IDirect3DCubeTexture9*)m_pcActualTextureRight; }
	bool IsStereo() { return (m_pcActualTextureRight != NULL); }
protected:
	/**
	* Wrapped Surface levels.
	***/
	std::unordered_map<CubeSurfaceKey, IDirect3DStereoSurface9*, hash_CubeSurfaceKey> m_aaWrappedSurfaceLevels;

	//Special handling required for locking rectangles if we are using Dx9Ex
	std::mutex m_mtx;
	std::unordered_map<UINT, bool> newSurface;
	IDirect3DCubeTexture9* lockableSysMemTexture;
};

/**
* Direct 3D proxy volume class.
* Imbeds the wrapped container this volume is part of.
*
* See D3D9ProxySurface for notes on reference counting when in container.
* @see D3D9ProxySurface
***/
class IDirect3DStereoVolume9 : public IDirect3DVolume9
{
public:
	IDirect3DStereoVolume9(IDirect3DVolume9* pcActualVolume, IDirect3DDevice9* pcOwningDevice, IUnknown* pcWrappedContainer) :
		m_pcActualVolume(pcActualVolume),
		m_pcOwningDevice(pcOwningDevice),
		m_pcWrappedContainer(pcWrappedContainer)
	{
		assert(pcOwningDevice != NULL);
	}

	/**
	* Destructor.
	* (else - m_pWrappedContainer does not have released called on it because the container manages
	* the device reference)
	***/
	~IDirect3DStereoVolume9()
	{
		SAFE_RELEASE(m_pcActualVolume);
	}

	/**
	* Base QueryInterface functionality.
	***/
	HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv)
	{
		return m_pcActualVolume->QueryInterface(riid, ppv);
	}

	/**
	* Behaviour determined through observing D3D with various test cases.
	*
	* Creating a volume should only increase the device ref count iff the volume has no parent container.
	* (The container adds one ref to the device for it and all its volumes)

	* If a volume has a container then adding references to the volume should increase the ref count on
	* the container instead of the volume. The volume shares a total ref count with the container, when
	* it reaches 0 the container and its volumes are destroyed. This is handled by sending all Add/Release
	* on to the container when there is one.
	***/
	ULONG WINAPI AddRef()
	{
		// if volume is in a container increase count on container instead of the volume
		if (m_pcWrappedContainer)
		{
			return m_pcWrappedContainer->AddRef();
		}
		else
		{
			// otherwise track references normally
			return ++m_unRefCount;
		}
	}

	/**
	* Releases wrapped container if present else the base surface.
	***/
	ULONG WINAPI Release()
	{
		if (m_pcWrappedContainer)
		{
			return m_pcWrappedContainer->Release();
		}
		else if (--m_unRefCount == 0)
		{
			delete this;
			return 0;
		}

		return m_unRefCount;
	}

	/**
	* GetDevice on the underlying IDirect3DVolume9 will return the device used to create it.
	* Which is the actual device and not the wrapper. Therefore we have to keep track of the wrapper
	* device and return that instead.

	* Calling this method will increase the internal reference count on the IDirect3DDevice9 interface.
	* Failure to call IUnknown::Release when finished using this IDirect3DDevice9 interface results in a
	* memory leak.
	***/
	HRESULT WINAPI GetDevice(IDirect3DDevice9** ppDevice)
	{
		if (!m_pcOwningDevice)
			return D3DERR_INVALIDCALL;
		else
		{
			*ppDevice = m_pcOwningDevice;
			s_bDeviceInUseByProxy = true;
			m_pcOwningDevice->AddRef();
			s_bDeviceInUseByProxy = false;
			return D3D_OK;
		}
	}

	/**
	* Base SetPrivateData functionality.
	***/
	HRESULT WINAPI SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
	{
		return m_pcActualVolume->SetPrivateData(refguid, pData, SizeOfData, Flags);
	}

	/**
	* Base GetPrivateData functionality.
	***/
	HRESULT WINAPI GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
	{
		return m_pcActualVolume->GetPrivateData(refguid, pData, pSizeOfData);
	}

	/**
	* Base FreePrivateData functionality.
	***/
	HRESULT WINAPI FreePrivateData(REFGUID refguid)
	{
		return m_pcActualVolume->FreePrivateData(refguid);
	}

	/**
	* Provides acces to parent object.
	* "Provides access to the parent cube texture or texture (mipmap) object, if this surface is a child
	* level of a cube texture or a mipmap. This method can also provide access to the parent swap chain
	* if the surface is a back-buffer child."
	*
	* "If the surface is created using CreateRenderTarget or CreateOffscreenPlainSurface or
	* CreateDepthStencilSurface, the surface is considered stand alone. In this case, GetContainer
	* will return the Direct3D device used to create the surface."
	* <http://msdn.microsoft.com/en-us/library/windows/desktop/bb205893%28v=vs.85%29.aspx>
	*
	* If the call succeeds, the reference count of the container is increased by one.
	* @return Owning device if no wrapped container present, otherwise the container.
	* @see D3D9ProxySurface::GetContainer()
	***/
	HRESULT WINAPI GetContainer(REFIID riid, LPVOID* ppContainer)
	{
		if (!m_pcWrappedContainer)
		{
			s_bDeviceInUseByProxy = true;
			m_pcOwningDevice->AddRef();
			s_bDeviceInUseByProxy = false;
			*ppContainer = m_pcOwningDevice;
			return D3D_OK;
		}


		void *pContainer = NULL;
		HRESULT queryResult = m_pcWrappedContainer->QueryInterface(riid, &pContainer);

		if (SUCCEEDED(queryResult))
		{
			*ppContainer = m_pcWrappedContainer;
			m_pcWrappedContainer->AddRef();

			return D3D_OK;
		}
		else if (queryResult == E_NOINTERFACE)
		{

			return E_NOINTERFACE;
		}
		else
		{
			return D3DERR_INVALIDCALL;
		}
	}

	/**
	* Base GetDesc functionality.
	***/
	HRESULT WINAPI GetDesc(D3DVOLUME_DESC *pDesc)
	{
		return m_pcActualVolume->GetDesc(pDesc);
	}

	/**
	* Base LockBox functionality.
	***/
	HRESULT WINAPI LockBox(D3DLOCKED_BOX *pLockedVolume, const D3DBOX *pBox, DWORD Flags)
	{
		return m_pcActualVolume->LockBox(pLockedVolume, pBox, Flags);
	}

	/**
	* Base UnlockBox functionality.
	***/
	HRESULT WINAPI UnlockBox()
	{
		return m_pcActualVolume->UnlockBox();
	}

	/**
	* Gets the actual (parent) volume.
	***/
	IDirect3DVolume9* GetActualVolume()
	{
		return m_pcActualVolume;
	}

protected:
	/**
	* Container this Volume is part of.
	* VolumeTexture, (other?) NULL if standalone.
	***/
	IUnknown* m_pcWrappedContainer;
	/**
	* The owning device.
	***/
	IDirect3DDevice9* m_pcOwningDevice;
	/**
	* The actual volume embedded.
	***/
	IDirect3DVolume9* m_pcActualVolume;
	/**
	* Internal reference counter.
	***/
	ULONG m_unRefCount;
};

/**
*  Direct 3D proxy stereo volume texture class.
*  Imbeds wrapped volume levels.
*/
class IDirect3DStereoVolumeTexture9 : public IDirect3DVolumeTexture9, public IDirect3DStereoBaseTexture9
{
public:
	/**
	* Constructor.
	* @param pcActualTexture Imbed actual texture.
	***/
	IDirect3DStereoVolumeTexture9(IDirect3DVolumeTexture9* pcActualTexture, IDirect3DDevice9* pcOwningDevice, D3DPOOL ePoolDefault) :
		IDirect3DStereoBaseTexture9(pcActualTexture, NULL, pcOwningDevice),
		m_aaWrappedVolumeLevels(),
		lockableSysMemVolume(NULL),
		m_ePoolDefault(ePoolDefault)
	{
	}

	/**
	* Destructor.
	* Releases embedded stuff.
	***/
	~IDirect3DStereoVolumeTexture9()
	{
		SHOW_CALL("IDirect3DStereoVolumeTexture9::~D3D9ProxyVolumeTexture");

		// delete all surfaces in m_levels
		auto it = m_aaWrappedVolumeLevels.begin();
		while (it != m_aaWrappedVolumeLevels.end())
		{
			// we have to explicitly delete the Volume here as the Release behaviour of the volume would get stuck in a loop
			// calling back to the container Release.
			delete it->second;
			it = m_aaWrappedVolumeLevels.erase(it);
		}

		if (lockableSysMemVolume)
			lockableSysMemVolume->Release();

		// call base desctructor
		IDirect3DStereoBaseTexture9::~IDirect3DStereoBaseTexture9();
	}

	/*** IUnknown methods ***/

	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv) { return IDirect3DStereoBaseTexture9::QueryInterface(riid, ppv); }
	virtual ULONG WINAPI AddRef() { return IDirect3DStereoBaseTexture9::AddRef(); }
	virtual ULONG WINAPI Release() { return IDirect3DStereoBaseTexture9::Release(); }

	/*** IDirect3DBaseTexture9 methods	***/

	HRESULT WINAPI GetDevice(IDirect3DDevice9** ppcDevice) { return IDirect3DStereoBaseTexture9::GetDevice(ppcDevice); }
	HRESULT WINAPI SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags) { return IDirect3DStereoBaseTexture9::SetPrivateData(refguid, pData, SizeOfData, Flags); }
	HRESULT WINAPI GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData) { return IDirect3DStereoBaseTexture9::GetPrivateData(refguid, pData, pSizeOfData); }
	HRESULT WINAPI FreePrivateData(REFGUID refguid) { return IDirect3DStereoBaseTexture9::FreePrivateData(refguid); }
	DWORD WINAPI SetPriority(DWORD PriorityNew) { return IDirect3DStereoBaseTexture9::SetPriority(PriorityNew); }
	DWORD WINAPI GetPriority() { return IDirect3DStereoBaseTexture9::GetPriority(); }
	void WINAPI PreLoad() { IDirect3DStereoBaseTexture9::PreLoad(); }
	D3DRESOURCETYPE WINAPI GetType() { return IDirect3DStereoBaseTexture9::GetType(); }
	DWORD WINAPI SetLOD(DWORD LODNew) { return IDirect3DStereoBaseTexture9::SetLOD(LODNew); }
	DWORD WINAPI GetLOD() { return IDirect3DStereoBaseTexture9::GetLOD(); }
	DWORD WINAPI GetLevelCount() { return IDirect3DStereoBaseTexture9::GetLevelCount(); }
	HRESULT WINAPI SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType) { return IDirect3DStereoBaseTexture9::SetAutoGenFilterType(FilterType); }
	D3DTEXTUREFILTERTYPE WINAPI GetAutoGenFilterType() { return IDirect3DStereoBaseTexture9::GetAutoGenFilterType(); }
	void WINAPI GenerateMipSubLevels() { IDirect3DStereoBaseTexture9::GenerateMipSubLevels(); }

	/**
	* Base GetLevelDesc functionality.
	***/
	HRESULT WINAPI GetLevelDesc(UINT Level, D3DVOLUME_DESC *pDesc)
	{
		SHOW_CALL("IDirect3DStereoVolumeTexture9::GetLevelDesc");
		HRESULT nHr = m_pcActualTextureV->GetLevelDesc(Level, pDesc);
		pDesc->Pool = m_ePoolDefault;
		return nHr;
	}

	/**
	* If proxy volume is already stored on this level, return this one, otherwise create it.
	* To create a new stored volume level, call the method on both (left/right) actual volumes.
	***/
	HRESULT WINAPI GetVolumeLevel(UINT Level, IDirect3DVolume9** ppVolumeLevel)
	{
		SHOW_CALL("D3D9ProxyVolumeTexture::GetVolumeLevel");
		HRESULT finalResult;

		// Have we already got a Proxy for this surface level?
		if (m_aaWrappedVolumeLevels.count(Level) == 1)
		{ // yes

			// TODO Should we call through to underlying texture and make sure the result of doing this operation on the 
			// underlying texture would still be a success? (not if we don't have to, will see if it becomes a problem)

			*ppVolumeLevel = m_aaWrappedVolumeLevels[Level];
			(*ppVolumeLevel)->AddRef();

			finalResult = D3D_OK;
		}
		else
		{
			// Get underlying surfaces (stereo pair made from the surfaces at the same level in the left and right textues), 
			//  wrap, then store in m_wrappedSurfaceLevels and return the wrapped surface
			static IDirect3DVolume9* pActualVolumeLevel = NULL;
			pActualVolumeLevel = NULL;

			HRESULT result = m_pcActualTextureV->GetVolumeLevel(Level, &pActualVolumeLevel);

			if (SUCCEEDED(result))
			{
				static IDirect3DStereoVolume9* pWrappedVolumeLevel = NULL;
				pWrappedVolumeLevel = new IDirect3DStereoVolume9(pActualVolumeLevel, m_pcOwningDevice, (IDirect3DStereoBaseTexture9*)this);

				if (m_aaWrappedVolumeLevels.insert(std::pair<ULONG, IDirect3DStereoVolume9*>(Level, pWrappedVolumeLevel)).second)
				{
					// insertion of wrapped surface level into m_wrappedSurfaceLevels succeeded
					*ppVolumeLevel = pWrappedVolumeLevel;
					(*ppVolumeLevel)->AddRef();
					finalResult = D3D_OK;
				}
				else
				{
					// Failure to insert should not be possible. In this case we could still return the wrapped surface,
					// however, if we did and it was requested again a new wrapped instance will be returned and things would explode
					// at some point. Better to fail fast.
					OutputDebugStringA(__FUNCTION__);
					OutputDebugStringA("\n");
					OutputDebugStringA("Unable to store surface level.\n");
					assert(false);

					finalResult = D3DERR_INVALIDCALL;
				}
			}
			else
			{
				OutputDebugStringA(__FUNCTION__);
				OutputDebugStringA("\n");
				OutputDebugStringA("Error fetching actual surface level.\n");
				finalResult = result;
			}
		}

		return finalResult;
	}

	/**
	* Base LockBox functionality.
	***/
	HRESULT WINAPI LockBox(UINT Level, D3DLOCKED_BOX *pLockedVolume, const D3DBOX *pBox, DWORD Flags)
	{
		SHOW_CALL("D3D9ProxyVolumeTexture::LockBox");

		D3DVOLUME_DESC desc;
		m_pcActualTextureV->GetLevelDesc(Level, &desc);
		if (desc.Pool != D3DPOOL_DEFAULT)
		{
			//Can't really handle stereo for this, so just lock on the original texture
			return m_pcActualTextureV->LockBox(Level, pLockedVolume, pBox, Flags);
		}

		//Create lockable system memory surfaces
		HRESULT hr = D3DERR_INVALIDCALL;
		if (!lockableSysMemVolume)
		{
			s_bDeviceInUseByProxy = true;
			hr = m_pcOwningDevice->CreateVolumeTexture(desc.Width, desc.Height, desc.Depth, 1, 0,
				desc.Format, D3DPOOL_SYSTEMMEM, &lockableSysMemVolume, NULL);
			s_bDeviceInUseByProxy = false;
			if (FAILED(hr))
				return hr;
		}

		if (((Flags | D3DLOCK_NO_DIRTY_UPDATE) != D3DLOCK_NO_DIRTY_UPDATE) &&
			((Flags | D3DLOCK_READONLY) != D3DLOCK_READONLY))
			hr = m_pcActualTextureV->AddDirtyBox(pBox);

		hr = lockableSysMemVolume->LockBox(Level, pLockedVolume, pBox, Flags);
		if (FAILED(hr))
			return hr;

		return hr;
	}

	/**
	* Base UnlockBox functionality.
	***/
	HRESULT WINAPI UnlockBox(UINT Level)
	{
		SHOW_CALL("D3D9ProxyVolumeTexture::UnlockBox");
		D3DVOLUME_DESC desc;
		m_pcActualTextureV->GetLevelDesc(Level, &desc);
		if (desc.Pool != D3DPOOL_DEFAULT)
		{
			return m_pcActualTextureV->UnlockBox(Level);
		}

		// unlock the lockable resource
		HRESULT hr = lockableSysMemVolume->UnlockBox(Level);
		if (FAILED(hr))
			return hr;

		// and update the actual resource by the lockable
		s_bDeviceInUseByProxy = true;
		hr = m_pcOwningDevice->UpdateTexture(lockableSysMemVolume, m_pcActualTexture);
		s_bDeviceInUseByProxy = false;
		if (FAILED(hr))
			return hr;

		return hr;
	}

	/**
	* Base AddDirtyBox functionality.
	***/
	HRESULT WINAPI AddDirtyBox(const D3DBOX *pDirtyBox)
	{
		SHOW_CALL("D3D9ProxyVolumeTexture::AddDirtyBox");
		return m_pcActualTextureV->AddDirtyBox(pDirtyBox);
	}

	/**
	* Returns the actual volume texture.
	***/
	IDirect3DVolumeTexture9* GetActual()
	{
		return (IDirect3DVolumeTexture9*)m_pcActualTexture;
	}
protected:
	/**
	* Wrapped Volume levels.
	***/
	std::unordered_map<UINT, IDirect3DVolume9*> m_aaWrappedVolumeLevels;
	/**
	* The default pool of the resource.
	***/
	D3DPOOL const m_ePoolDefault;
	/**
	* Special handling required for locking boxes if we are using Dx9Ex
	***/
	IDirect3DVolumeTexture9* lockableSysMemVolume;
};

/**
*  Direct 3D vertex buffer class.
*  Overwrites IDirect3DVertexBuffer9 and imbeds the actual vertex buffer.
*/
class IDirect3DProxyVertexBuffer9 : public IDirect3DVertexBuffer9
{
public:
	/**
	* Constructor.
	* @param pActualVertexBuffer Imbed actual vertex buffer.
	* @param pOwningDevice Pointer to the device that owns the buffer.
	***/
	IDirect3DProxyVertexBuffer9::IDirect3DProxyVertexBuffer9(IDirect3DVertexBuffer9* pcActualVertexBuffer, IDirect3DDevice9* pcOwningDevice) :
		m_pcActualVertexBuffer(pcActualVertexBuffer),
		m_pcOwningDevice(pcOwningDevice),
		lockableSysMemBuffer(nullptr)
	{
		SHOW_CALL("IDirect3DProxyVertexBuffer9::IDirect3DProxyVertexBuffer9");

		assert(pcActualVertexBuffer != NULL);
		assert(pcOwningDevice != NULL);

		if (!pcActualVertexBuffer)
		{
			OutputDebugString(L"[STS] Trying to create a zero vertex buffer. \n");
			m_unRefCount = 1;
		}
		else
		{
			// set ref count by actual ref count
			pcActualVertexBuffer->AddRef();
			m_unRefCount = pcActualVertexBuffer->Release();
		}
	}

	/**
	* Destructor.
	* Releases embedded vertex buffer.
	***/
	IDirect3DProxyVertexBuffer9::~IDirect3DProxyVertexBuffer9()
	{
		SHOW_CALL("IDirect3DProxyVertexBuffer9::~IDirect3DProxyVertexBuffer9");

		if (m_pcActualVertexBuffer)
		{
			m_pcActualVertexBuffer->Release();
		}
	}

	/**
	* Base QueryInterface functionality.
	***/
	HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv)
	{
		SHOW_CALL("IDirect3DProxyVertexBuffer9::QueryInterface");

		return m_pcActualVertexBuffer->QueryInterface(riid, ppv);
	}

	/**
	* Base AddRef functionality.
	***/
	ULONG WINAPI AddRef()
	{
		SHOW_CALL("IDirect3DProxyVertexBuffer9::AddRef");

		return ++m_unRefCount;
	}

	/**
	* Base Release functionality.
	***/
	ULONG WINAPI Release()
	{
		SHOW_CALL("IDirect3DProxyVertexBuffer9::Release");

		if (--m_unRefCount == 0)
		{
			delete this;
			return 0;
		}

		return m_unRefCount;
	}

	/**
	* Base GetDevice functionality.
	***/
	HRESULT WINAPI GetDevice(IDirect3DDevice9** ppDevice)
	{
		SHOW_CALL("IDirect3DProxyVertexBuffer9::GetDevice");

		if (!m_pcOwningDevice)
			return D3DERR_INVALIDCALL;
		else
		{
			*ppDevice = m_pcOwningDevice;
			s_bDeviceInUseByProxy = true;
			m_pcOwningDevice->AddRef();
			s_bDeviceInUseByProxy = false;
			return D3D_OK;
		}
	}

	/**
	* Base SetPrivateData functionality.
	***/
	HRESULT WINAPI SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
	{
		SHOW_CALL("IDirect3DProxyVertexBuffer9::SetPrivateData");

		return m_pcActualVertexBuffer->SetPrivateData(refguid, pData, SizeOfData, Flags);
	}

	/**
	* Base GetPrivateData functionality.
	***/
	HRESULT WINAPI GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData)
	{
		SHOW_CALL("IDirect3DProxyVertexBuffer9::GetPrivateData");

		return m_pcActualVertexBuffer->GetPrivateData(refguid, pData, pSizeOfData);
	}

	/**
	* Base FreePrivateData functionality.
	***/
	HRESULT WINAPI FreePrivateData(REFGUID refguid)
	{
		SHOW_CALL("IDirect3DProxyVertexBuffer9::FreePrivateData");

		return m_pcActualVertexBuffer->FreePrivateData(refguid);
	}

	/**
	* Base SetPriority functionality.
	***/
	DWORD WINAPI SetPriority(DWORD PriorityNew)
	{
		SHOW_CALL("IDirect3DProxyVertexBuffer9::SetPriority");

		return m_pcActualVertexBuffer->SetPriority(PriorityNew);
	}

	/**
	* Base GetPriority functionality.
	***/
	DWORD WINAPI GetPriority()
	{
		SHOW_CALL("IDirect3DProxyVertexBuffer9::GetPriority");
		return m_pcActualVertexBuffer->GetPriority();
	}

	/**
	* Base PreLoad functionality.
	***/
	void WINAPI PreLoad()
	{
		SHOW_CALL("IDirect3DProxyVertexBuffer9::PreLoad");

		return m_pcActualVertexBuffer->PreLoad();
	}

	/**
	* Base GetType functionality.
	***/
	D3DRESOURCETYPE WINAPI GetType()
	{
		SHOW_CALL("IDirect3DProxyVertexBuffer9::GetType");

		return m_pcActualVertexBuffer->GetType();
	}

	/**
	* Base Lock functionality.
	***/
	HRESULT WINAPI Lock(UINT OffsetToLock, UINT SizeToLock, VOID **ppbData, DWORD Flags)
	{
		SHOW_CALL("IDirect3DProxyVertexBuffer9::Lock");

		return m_pcActualVertexBuffer->Lock(OffsetToLock, SizeToLock, ppbData, Flags);
	}

	/**
	* Base Unlock functionality.
	***/
	HRESULT WINAPI Unlock()
	{
		SHOW_CALL("IDirect3DProxyVertexBuffer9::Unlock");

		return m_pcActualVertexBuffer->Unlock();
	}

	/**
	* Base GetDesc functionality.
	***/
	HRESULT WINAPI GetDesc(D3DVERTEXBUFFER_DESC *pDesc)
	{
		SHOW_CALL("IDirect3DProxyVertexBuffer9::GetDesc");

		return m_pcActualVertexBuffer->GetDesc(pDesc);
	}

	/**
	* Returns the actual embedded buffer pointer.
	***/
	IDirect3DVertexBuffer9* GetActual()
	{
		return m_pcActualVertexBuffer;
	}

protected:
	/**
	* The actual vertex buffer embedded.
	***/
	IDirect3DVertexBuffer9* const m_pcActualVertexBuffer;
	/**
	* Pointer to the D3D device that owns the buffer.
	***/
	IDirect3DDevice9* m_pcOwningDevice;
	/**
	* Internal reference counter.
	***/
	ULONG m_unRefCount;
	/**
	* Special handling required for locking buffers if we are using Dx9Ex
	***/
	IDirect3DVertexBuffer9* lockableSysMemBuffer;
};
