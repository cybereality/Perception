#VIREIO PERCEPTION

![alt app screenshot](https://github.com/DreamNik/Perception/blob/new_gui/wiki/app_screenshot.png)

####Requirements

1. Created using Visual C++ 2012 Express for Windows Desktop on Windows 8 64-bit.
   http://www.microsoft.com/en-us/download/details.aspx?id=34673 "Microsoft"
   Windows 8.1 users cant use the online installer, please use the ISO file instead.

2. Built using the Microsoft DirectX SDK June 2010 (may work with newer versions)
   http://www.microsoft.com/en-au/download/details.aspx?id=6812

3. Download libfreespace-0.6rc0-win32-vs2010.zip from
   https://launchpad.net/libfreespace/+download
   Extract it to a folder

4. Download Windows Driver Development Kit 7.1.0 from
   http://www.microsoft.com/en-gb/download/details.aspx?id=11800
   Install to a suitable folder, no need to install all the samples or documentation, it is only required for the ATL header files and libraries

5. Download Oculus SDK v0.4.2 from
   https://developer.oculusvr.com/
   Install it to a folder

6. Download and extract FFMPEG Shared  and  dev build (32 bit) from
   http://ffmpeg.zeranoe.com/builds/
   Extract it to a folder

7. Download and install Qt framework from
   http://qt-project.org/downloads
   Install Qt 5.x.x 32-bit VS 2012 version.


####BUILD INSTRUCTIONS:

1. Set macros in "common/project/paths.props" file:
    * FREESPACE   : The folder for libfreespace (Hillcrest Labs SDK).
    * DXSDK_DIR   : The folder for DirectX SDK (already set automatically by the June 2010 DirectX SDK)
    * WIN_DDK_DIR : The folder you installed the Windows Driver Development Kit to
    * FFMPEG      : The folder you installed the FFMPEG library
    * LIBOVR      : The "LibOVR" folder of Oculus SDK library
	* QTDIR       : The folder for Qt framework, which contains "bin" , "include" and "lib" directories. (usually resides in subfolder of "5.x" directory of install directory).
	

2. Create "Vireio/Perception" folder (output folder) in Program Files with full permissions for current user
   or change "OutDir" value in file "VireioPerception.props" to any suitable folder.
   To change permissions of folder, open it's properties, go to "Security" tab,
   click "Advanced", click "Change permissions", click "Disable inheritance",
   select "Remove all inherited permissions...", check "Replace all child ...", click "Add",
   click "Select principal", type "Users", click "OK", check "Full controll", click "OK",
   click "OK", click "Yes", click "OK".

3. Copy contents of "data" folder to created folder.

4. Open the VireioPerception.sln solution file. It contains all the projects with their dependencies set correctly. 

5. Choose either Debug or Release. 

6. Either Run or Build the solution. It should just work.

7. Copy contents of "data" folder to output folder.

When you want to publish it, build in Release mode, remove *.lib, *.pdb, *.exp files from output folder (look in "bin" and "cfg/VRboost_rules" directories).

####Support

If you want to view the debug prints from hooking the game, download and run [DebugView](http://technet.microsoft.com/en-au/sysinternals/bb896647.aspx "Microsoft") or [TraceSpy](http://tracespy.codeplex.com/). 
Sometimes DebugView becomes unresponsive, in which case just continue what you were doing and look at it later.

The VireIO_Proxy project is the main meat of the application. Basically what it does is it provides wrapped DirectX interface. Mostly all the functions are just dumb pass-throughs, but a few key functions are modified. This allows us to alter a few things, like the view-projection matrix (which is modified to fake a 3D camera), or the present call (where we display a new 3D image). 

The VireIO_Hijack project redirects api calls to DirectX to wrapped VireIO_Proxy interface.

The VireIO_Perception project is the GUI interface for the app. You probably will not need to touch this unless you are adding new options to the menu (ie a new tracking device).

If you need help, please post on the Official Vireio Perception Forums on [MTBS3D.com](http://www.mtbs3d.com/phpBB/viewforum.php?f=141).
