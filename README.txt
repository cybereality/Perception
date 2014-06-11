---------------------------------
////    VIREIO PERCEPTION    ////
---------------------------------

BUILD INSTRUCTIONS:
---------------------------------
Created using Visual C++ 2012 Express for Windows Desktop on Windows 8 64-bit. 
(Windows 8.1 users cant use the online installer, please use the ISO file instead)
http://www.microsoft.com/en-us/download/confirmation.aspx?id=34673 

Built using the Microsoft DirectX SDK (June 2010) (may work with newer versions).
http://www.microsoft.com/en-au/download/details.aspx?id=6812

Download libfreespace (libfreespace-0.6rc0-win32-vs2010.zip) from here:
https://launchpad.net/libfreespace/+download
Extract it to a folder

Download Oculus SDK v0.2.5c from
https://developer.oculusvr.com/
Extract and copy LibOVR to project directory.

Click Start, Control Panel, System (in System and Security), Advanced System Settings, Environment Variables
Create environment variables:
	FREESPACE : The folder for libfreespace (Hillcrest Labs SDK).
	DXSDK_DIR : Your DirectX SDK folder (already set automatically by the June 2010 DirectX SDK)

Open the VireioPerception.sln solution file. It contains all the projects with their dependencies set correctly. 
Choose either Debug or Release. 
Either Run or Build the solution.
It should just work.
(There should be only two compiler warnings about the output directories not being the same as the target directory. That's deliberate.)

When you want to publish it, just build in Release mode and zip the contents of the Release folder. 
But be careful because when running in Release mode, changing the separation, convergence or other settings in game will affect the files you will release.
The Debug folder is automatically created from the files in the Release folder whenever you compile in Debug mode. 

If you want to view the debug prints from hooking the game, download and run DebugView or TraceSpy. 
http://technet.microsoft.com/en-au/sysinternals/bb896647.aspx
http://tracespy.codeplex.com/
Sometimes DebugView becomes unresponsive, in which case just continue what you were doing and look at it later.

It makes sense to work with the DxProxy project mainly. The DxProxy project is the main meat of the application. Basically what it does is it proxies DirectX so the game loads this modified "d3d9.dll" file instead of the real "d3d9.dll". Mostly all the functions are just dumb pass-throughs, but a few key functions are modified. This allows us to alter a few things, like the view-projection matrix (which is modified to fake a 3D camera), or the present call (where we display a new 3D image). 

The DxHijack project helps us to inject our modified dll into games, without having to place the proxy "d3d9.dll" file in the game folder (though some games don't work with this method, so we still have to copy the dll over as a last resort). 

The Perception project is the GUI interface for the app. You probably will not need to touch this unless you are adding new options to the menu (ie a new tracking device).

If you need help, please post on the Official Vireio Perception Forums on MTBS3D.com.
http://www.mtbs3d.com/phpBB/viewforum.php?f=141
