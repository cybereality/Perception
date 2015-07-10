#VIREIO PERCEPTION - Plugin Section



####BUILD INSTRUCTIONS



Created using [Visual C++ 2012 Express for Windows Desktop](http://www.microsoft.com/en-us/download/details.aspx?id=34673 "Microsoft") on Windows 8 64-bit.  





Built using the [Microsoft DirectX SDK (June 2010)](http://www.microsoft.com/en-au/download/details.aspx?id=6812 "Microsoft") (may work with newer versions)



Download [Oculus SDK v0.4.4](https://developer.oculusvr.com "Oculus VR")


1. Click Start, Control Panel, System (in System and Security), Advanced System Settings, Environment Variables

Create environment variables:

    * DXSDK_DIR : Your DirectX SDK folder (already set automatically by the June 2010 DirectX SDK)

    * OVRSDKROOT : Your Oculus SDK folder

2. Open the VireioPlugins.sln solution file. It contains all the Plugin projects with their dependencies set correctly. 

3. Choose either Debug or Release. 

4. Build the solution. It should compile to {User}\documents\my games\aquilinus\my plugins.  



####Support



If you want to view the debug prints from hooking the game, download and run [DebugView](http://technet.microsoft.com/en-au/sysinternals/bb896647.aspx "Microsoft") or [TraceSpy](http://tracespy.codeplex.com/). 

Sometimes DebugView becomes unresponsive, in which case just continue what you were doing and look at it later.



If you need help, please post on the Official Vireio Perception Forums on [MTBS3D.com](http://www.mtbs3d.com/phpBB/viewforum.php?f=141).



####Plugin List

* Vireio Stereo Splitter - DevNr 2006 - PluginNr 64
* Vireio Stereo Presenter - DevNr 2006 - PluginNr 65



* Vireio Cinema - DevNr 2006 - PluginNr 128

* Vireio Oculus Tracker - DevNr 2006 - PluginNr 256

* Vireio Oculus Renderer - DevNr 2006 - PluginNr 258

* Vireio Oculus Direct Mode - DevNr 2006 - PluginNr 259

