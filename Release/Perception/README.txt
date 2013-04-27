---------------------------------
////    VIREIO PERCEPTION    ////
---------------------------------

HOT-KEYS:
---------------------------------
F1 : Save Screenshot.
F2 / F3 : Adjust Separation.
SHIFT + F2 / F3 : Adjust Camera Offset.
F4 / F5 : Adjust Convergence.
F6 : Swap Eyes.
SHIFT + F6 : Reset Settings
F8 / F9 : Adjust Tracking Yaw.
SHIFT + F8 / F9 : Adjust Tracking Pitch.
CONTROL + F8 / F9 : Adjust Tracking Roll.
NumPad 0 : enable/disable stereo mode


Schneider-Hicks Optical Calibration Tool (S.H.O.C.T.)
-------------------------------------------------------

Stereoscopic 3D drivers make it possible for existing PC games to work with modern VR head mounted displays like the upcoming Oculus Rift (and current Rift SDK). The problem with head mounted displays is it's impossible to observe what each eye is seeing in a way that makes proper 3D calibration easy to do. Furthermore, it's difficult to easily determine if the user's eyes are being forced to diverge which is painful and potentially unhealthy to do.

SHOCT is a new feature that has been added to the Vireio Perception drivers and was invented by Neil Schneider and John Hicks. Based on a movable visual indicator, users are able to accurately mark the placement of their left and right pupil. This marker is then used as a calibration tool to comfortably determine the level of 3D separation in the game based on what the user's eye placement is actually capable of.

The benefits of the SHOCT feature include comfortable VR gaming, effective stereoscopic 3D settings, and a potentially healthier and safer way to game with head mounted displays - especially when dealing with content that wasn't custom designed for the VR experience.

Visit Meant to be Seen (http://www.mtbs3D.com) for complete instructions.

Related hotkeys:

SHIFT + * (NumPad) : Cycle the Schneider-Hicks Optical Calibration Tool(S.H.O.C.T.)
NumPad 1 & 2: Move left SHOCT line
NumPad 4 & 5 : Move right SCHOCT line

Non NumPad keys:
ctrl+I : Cycle the Schneider-Hicks Optical Calibration Tool(S.H.O.C.T.)
O,P,K,L to adjust SCHOCT line.


SUPPORTED GAMES (Rating):
---------------------------------
Half-Life 2 	(Great)
Left4Dead	(Great)
Left4Dead 2	(Great)
Dear Esther	(Great)
Antichamber	(Good)
Dishonored	(Good)
Mirror's Edge	(Good)
AaaaAAAAA!!!!!	(Good)
Skyrim 		(Good)
Portal		(Poor)
Portal 2	(Poor)
DiRT 2		(Poor)
UT3		(Poor)

NOTES:
---------------------------------
- For some games, you only need to leave the Perception app open when you launch the game. For other games you may need to follow specific instructions, see below.

- For best performance, be sure to disable vertical sync.

- It is better to adjust all game settings before playing with the driver, as changing certain settings may cause the game to crash (i.e. changing the resolution).

- With formats that split the screen (side-by-side, etc.) the mouse icon may not be located with where the cursor actually is. 

- If the app crashed when you select a display mode, you may want to right-click the Perception.exe and choose "Run as Administrator"

- SLI and CrossfireX may cause strange flickering on some titles (ie L4D2).

- It is NOT recommended that you play online games since the driver could be detected as a hack and you may be banned.

- If the Hillcrest Labs tracker appears unresponsive, try shaking it as the game loads. 

- Some games produce extreme lag when using the Hillcrest Labs tracker. In those cases you must disable tracking and use the built-in mouse emulation.

GAME NOTES:
---------------------------------
Dishonored: Copy "bin/d3d9.dll" and "libfreespace.dll" to game's directory.

Half-Life 2: Set Shadow Detail to Medium. Enable developers console. Use "fov_desired 110".

Left4Dead: Try launching left4dead.exe from game directory (if not working normally). Download and extract "viewmodeloverride(3).zip" to "left4dead/addons". Use "fov_override 110".

Left4Dead 2: Try launching left4dead2.exe from game directory (if not working normally).

DeadEsther: Use "fov_desired 110".

Portal 2: Use "r_shadows 0", "cl_fov 110", "viewmodel_offset_y 8"

Mirror's Edge: Lanuch "MirrorsEdge.exe" from "mirrors edge/Binaries"

AaaaAAAA!!!!: Copy "bin/d3d9.dll" to game's directory.

Unreal Tournament 3: Launch the UT3.exe from the game's directory (Binaries). Screen freezes with some full-screen shaders.

Skyrim: Disable shadows by finding "SkyrimPrefs.ini" in "My Documents\My Games\Skyrim" and setting the following: "fShadowDistance=0.0000 fInteriorShadowDistance=0.0000 fShadowLODStartFade=0.0000". Open the developers console and use "FOV 110".

DiRT2: Some objects incorrect in first person dashboard mode.  Make sure to force directx 9 in "My Documents\My Games\DiRT2\hardwaresettings\hardware_settings_config.xml"
