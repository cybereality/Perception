---------------------------------
////    VIREIO PERCEPTION    ////
---------------------------------

BUILD INSTRUCTIONS:
---------------------------------
Created using Visual C++ 2010 Express (may work with newer versions) on Windows 7 64-bit. 

Built using the Microsoft DirectX SDK (June 2010) (may work with newer versions).

Create environment variables:
	DX : Your DirectX SDK folder.
	DX_PROXY : The DxProxy source folder (with the cpp files).
	DX_HIJACK : The DxHijack source folder (with the cpp files).
	FREESPACE : The folder for libfreespace (Hillcrest Labs SDK).
	
Open the DxProxy.sln solution file and build that first. You may have to create an output folder first, it defaults to "C:/Program Files(x86)/Vireio/Perception". The best thing to do is to download the binaries to that folder (so the directory structure is set up).

Then open DxHijack.sln and build that next. It depends on the files in DxProxy, though it makes more sense to work with the DxProxy project mainly, and then just make sure to build both projects when you are ready.

The DxProxy project is the main meat of the application. Basically what it does is it proxies DirectX so the game loads this modified "d3d9.dll" file instead of the real "d3d9.dll". Mostly all the functions are just dumb pass-throughs, but a few key functions are modified. This allows us to alter a few things, like the view-projection matrix (which is modified to fake a 3D camera), or the present call (where we display a new 3D image). 

The DxHijack project helps us to inject our modified dll into games, without having to place the proxy "d3d9.dll" file in the game folder (though some games don't work with this method, so we still have to copy the dll over as a last resort). 

The Perception project is the GUI interface for the app. You probably will not need to touch this unless you are adding new options to the menu (ie a new tracking device).

If you need help, please post on the Official Vireio Perception Forums on MTBS3D.com.