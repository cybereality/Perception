# Aquilinus - Vireio Perception 3D Modification Studio

Aquilinus is an injected DLL that redirects GPU API calls to modify games for Vireio Perception v4. 

It has a built-in, fully node-based working area. Any part of Vireio Perception (HDM APIs, Stereo Splitter, Matrix Modifier,....) actually is a Plugin-Node, these nodes eventually get connected to build the different Vireio Perception game profiles.

#### Build Instructions

Build using Visual Studio 2019 (C++).

Download and install GL, GLEW, GLFW3, DirectXMath.

You can use MS-VCPKG ( https://github.com/Microsoft/vcpkg ) to download the different packages.

#### Dear ImGui

Aquilinus uses Dear ImGui grapical user interface and the ImNodes extension.

Go to 
https://github.com/ocornut/imgui
https://github.com/rokups/ImNodes/blob/master/ImNodes.h
for further info.