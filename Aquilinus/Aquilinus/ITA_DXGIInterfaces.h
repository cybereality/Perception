/********************************************************************
Vireio Perception : Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Aquilinus : Vireio Perception 3D Modification Studio 
Copyright © 2014 Denis Reischl

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
#ifndef ITA_DXGIINTERFACES_ENUM
#define ITA_DXGIINTERFACES_ENUM

namespace ITA_DXGIINTERFACES
{
	/**
	* IDXGI interfaces enumeration.
	* The following interfaces are used with Direct3D 10+.
	***/
	enum ITA_DXGIInterfaces
	{
		/*** DXGI core interfaces ***/
		IDXGIAdapter,           /**< Represents a display sub-system (including one or more GPUs, DACs and video memory). **/
		IDXGIAdapter1,          /**< Represents a display sub-system (including one or more GPUs, DACs and video memory). **/
		IDXGIAdapter2,          /**< Represents a display sub-system (including one or more GPUs, DACs and video memory). The interface exposes a method to get a DXGI 1.2 description of an adapter or video card. **/
		IDXGIDebug,             /**< Controls debug settings, and can only be used if the debug layer is turned on. **/
		IDXGIDebug1,            /**< Controls debug settings for DXGI. You can use the IDXGIDebug1 interface in Windows Store apps. **/
		IDXGIDecodeSwapChain,   /**< Represents a swap chain that is used by desktop media apps to decode video data and show it on a DirectComposition surface. **/
		IDXGIDevice,            /**< Implements a derived class for DXGI objects that produce image data. **/
		IDXGIDevice1,           /**< Implements a derived class for DXGI objects that produce image data. **/
		IDXGIDevice2,           /**< Implements a derived class for DXGI objects that produce image data. The interface exposes methods to block CPU processing until the GPU completes processing, and to offer resources to the operating system. **/
		IDXGIDevice3,           /**< Implements a derived class for DXGI objects that produce image data. The interface exposes a method to trim graphics memory usage by the DXGI device. **/
		IDXGIDeviceSubObject,   /**< Inherited from objects that are tied to the device so that they can retrieve a pointer to it. **/
		IDXGIDisplayControl,    /**< Exposes methods to indicate user preference for the operating system's stereoscopic 3D display behavior and to set stereoscopic 3D display status to enable or disable. **/
		IDXGIFactory,           /**< Implements methods for generating DXGI objects. **/
		IDXGIFactory1,          /**< Implements methods for generating DXGI objects. **/
		IDXGIFactory2,          /**< Includes methods to create a newer version swap chain with more features than IDXGISwapChain and to monitor stereoscopic 3D capabilities. **/
		IDXGIFactory3,          /**< Enables creating DXGI objects. **/
		IDXGIFactoryMedia,      /**< Creates swap chains for desktop media apps that use DirectComposition surfaces to decode and display video. **/
		IDXGIInfoQueue,         /**< Controls the debug information queue, and can only be used if the debug layer is turned on. **/
		IDXGIKeyedMutex,        /**< Represents a keyed mutex, which allows exclusive access to a shared surface that is used by multiple devices. **/
		IDXGIObject,            /**< Implements a base class for all DXGI objects with methods for getting and setting data and retrieving parent interfaces. **/
		IDXGIOutput,            /**< Represents an adapter output (such as a monitor). **/
		IDXGIOutput1,           /**< Represents an adapter output (such as a monitor). Extends IDXGIOutput to support stereo display mode and desktop duplication. **/
		IDXGIOutput2,           /**< Represents an adapter output (such as a monitor). Exposes a method to check for multipane overlay support on the primary output adapter. **/
		IDXGIOutputDuplication, /**< Accesses and manipulates the duplicated desktop image. **/
		IDXGIResource,          /**< Allows resource sharing and identifies the memory that a resource resides in. **/
		IDXGIResource1,         /**< Extends the IDXGIResource interface by adding support for creating a subresource surface object and for creating a handle to a shared resource. **/
		IDXGISurface,           /**< Implements methods for image-data objects. **/
		IDXGISurface1,          /**< Extends the IDXGISurface interface by adding support for rendering to a DXGI interface by using GDI. **/
		IDXGISurface2,          /**< Extends the IDXGISurface1 interface by adding support for subresource surfaces and getting a handle to a shared resource. **/
		IDXGISwapChain,         /**< Implements one or more surfaces for storing rendered data before presenting it to an output. **/
		IDXGISwapChain1,        /**< Provides presentation capabilities that are enhanced from IDXGISwapChain. **/
		IDXGISwapChain2,        /**< Extends IDXGISwapChain1 with methods to set and retrieve the subregion of the back buffer used for the swap chain. **/
		IDXGISwapChain3,        /**< Extends IDXGISwapChain2 with methods to support getting the index of the swap chain's current back buffer and support for color space. **/
		IDXGISwapChainMedia,    /**< This swap chain interface allows desktop media applications to request a seamless change to a specific refresh rate. **/
	};

}
#endif