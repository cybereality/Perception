#pragma once

template<class T>
class cBase : public T{
public:

	cBase( T* a , D3DProxyDevice* d , IUnknown* c=0 ){
		actual    = a;
		device    = d;
		refCount  = 1;
		container = c;

		if( !container ){
			device->AddRef();
		}

		// else - We leave the device ref count changes to the container

		// pWrappedContainer->AddRef(); is not called here as container add/release is handled
		// by the container. The ref could be added here but as the release and destruction is
		// hanlded by the container we leave it all in the same place (the container)	
	}

	~cBase( ){
		actual->Release();

		if( !container ){
			device->Release();
		}
	}

	T*              actual;

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
	D3DProxyDevice* device;

	/**
	* Internal reference counter. 
	***/
	ULONG           refCount;

	/**
	* Container this surface is part of. Texture, CubeTexture, SwapChain, (other?) NULL if standalone surface.
	*
	* If a surface is in a container then they use the containers ref count as a shared total ref count.
	* The container is responsible for deleting any surfaces it contains when the ref count reaches 0.
	* Surfaces must be deleted before the container.
	***/
	IUnknown*       container;


	HRESULT WINAPI QueryInterface( REFIID riid , LPVOID* ppv ){
		return actual->QueryInterface( riid , ppv );
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
	ULONG WINAPI AddRef(){
		if( container ){
			return container->AddRef();
		}
		return ++refCount;
	}


	/**
	* Releases wrapped container if present else the base surface.
	***/
	ULONG WINAPI Release(){
		if( container ){
			return container->Release();
		}
		if( --refCount == 0 ){
			delete this;
			return 0;
		}

		return refCount;
	}

	/**
	* GetDevice on the underlying IDirect3DCubeTexture9 will return the device used to create it. 
	* Which is the actual device and not the wrapper. Therefore we have to keep track of the 
	* wrapper device and return that instead.
	* 
	* Calling this method will increase the internal reference count on the IDirect3DDevice9 interface. 
	* Failure to call IUnknown::Release when finished using this IDirect3DDevice9 interface results in a 
	* memory leak.
	*/
	HRESULT WINAPI GetDevice( IDirect3DDevice9** ppDevice ){
		if( !device ){
			return D3DERR_INVALIDCALL;
		}

		*ppDevice = device;
		device->AddRef();
		return D3D_OK;
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
	HRESULT WINAPI GetContainer(REFIID riid, LPVOID* ppContainer){
		if( !container ){
			device->AddRef();
			*ppContainer = device;
			return D3D_OK;
		}					


		void *pContainer = NULL;
		HRESULT queryResult = container->QueryInterface(riid, &pContainer);

		if( SUCCEEDED(queryResult) ){
			*ppContainer = container;
			container->AddRef();
			return D3D_OK;
		}
		
		if( queryResult == E_NOINTERFACE ){
			return E_NOINTERFACE;
		}

		return D3DERR_INVALIDCALL;
	}
};