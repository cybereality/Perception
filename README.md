![](https://github.com/cybereality/Perception/raw/master/Perception_v3/Release/Perception/img/logo.bmp)

# Vireio Perception v4

#### About :question:

Vireio Perception are open source VR drivers that let you take existing games and play them in VR.

Vireio Perception v4 is splitted into several parts, where every single part is its own plugin and 
acts as a node in a tool called "Aquilinus – 3D Modification Studio". This allows us to only include
parts needed into a unique game profile for the chosen game. And it allows us to specialize this
game profile depending on the supported hardware and user preference.

With Aquilinus friends of Vireio are able to create their own game profiles for their favorite
games in a node based working area. Use Inicio.exe to inject the Aquilinus workspace to the relevant
software.

Vireio Perception supports both D3D9 and D3D11.

Current list of Vireio Perception v4 nodes:

:white_small_square: Oculus Tracker *outdated*<br/>
:white_small_square: OpenVR Tracker *outdated*<br/>
:white_small_square: Oculus DirectMode *outdated*<br/>
:white_small_square: OpenVR DirectMode *outdated*<br/>
:white_small_square: VRBoost *closed source outdated*<br/>
:black_small_square: Vireio Constructor<br/>
:black_small_square: Vireio MatrixModifier<br/>
:black_small_square: Vireio MatrixModifierDx11<br/>
:black_small_square: Vireio StereoPresenter<br/>
:black_small_square: Vireio StereoSplitter<br/>
:black_small_square: Vireio StereoSplitterDx11<br/>
:black_small_square: Vireio Cinema<br/>

The basic principle is :
1. Splitter creates a second render target (making the game basically stereo) ->
2. Constructor (D3D11 only) helps the Splitter ->
3. MatrixModifier modifies the matrices stored in the shaders/buffers (making the game geometry stereo) ->
4. Cinema renders the stereo frame to either a virtual cinema room or directly close to the eyes ->
5. Presenter adds the in-game on-screen Vireio Menu and handles all options accordingly
6. VRBoost adding head-tracking directly to game memory registers

Vireio Perception v4 executeables :file_folder://Perception//bin <br/>
:sparkle: Perception.exe : main driver<br/>
:sparkle: Inicio.exe : Aquilinus workspace injection tool<br/>

#### Current state :calendar:

The drivers have been successful in providing game profiles for well known first person games. Since
the most desired games (Skyrim, Fallout 4, Borderlands,...) already provide native VR-support the
drivers lost importance. We reached our goal in showing the game studios / developers that their 
games do well in VR.

In 2021, the drivers were overworked, including full integration of Dear ImGui. That means that the
drivers are basically finished, with outdated support for HMDs. 

#### Licence :grey_exclamation:

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


#### Build Instructions :gear:

Created using [Visual Studio Community C++ 2019](https://visualstudio.microsoft.com/downloads/ "Microsoft") on Windows 10 64-bit.  

Download and install [GL](https://www.khronos.org/opengl/wiki/Getting_Started#Downloading_OpenGL), [GLEW](https://github.com/nigels-com/glew), [GLFW3](https://www.glfw.org/download.html), [DirectXMath](https://github.com/microsoft/DirectXMath) (if not already installed).

You can use [Microsoft vcpkg](https://github.com/Microsoft/vcpkg) to download the different packages.

Open and compile "//Perception//VS2019//VireioPerception.sln".

#### Support

If you need help, please post on the Official Vireio Perception Forums on [MTBS3D.com](https://www.mtbs3d.com/phpBB/viewforum.php?f=149 "MEANT-TO-BE-SEEN").

#### Dear ImGui

Vireio Perception uses [Dear ImGui](https://github.com/ocornut/imgui) grapical user interface and its [ImNodes](https://github.com/rokups/ImNodes/blob/master/ImNodes.h) extension.

