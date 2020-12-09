![](https://github.com/cybereality/Perception/raw/master/Perception_v3/Release/Perception/img/logo.bmp)

# Aquilinus - Vireio Perception 3D Modification Studio

Aquilinus is an injected DLL that redirects GPU API calls to modify games for Vireio Perception v4. 

It has a built-in, fully node-based working area. Any part of Vireio Perception (HDM APIs, Stereo Splitter, Matrix Modifier,....) actually is a Plugin-Node, these nodes eventually get connected to build the different Vireio Perception game profiles.

#### Build Instructions

Created using [Visual Studio Community C++ 2019](https://visualstudio.microsoft.com/downloads/ "Microsoft") on Windows 10 64-bit.  

Download and install [GL](https://www.khronos.org/opengl/wiki/Getting_Started#Downloading_OpenGL), [GLEW](https://github.com/nigels-com/glew), [GLFW3](https://www.glfw.org/download.html), [DirectXMath](https://github.com/microsoft/DirectXMath) (if not already installed).

You can use [Microsoft vcpkg](https://github.com/Microsoft/vcpkg) to download the different packages.

#### Dear ImGui

Vireio Perception uses [Dear ImGui](https://github.com/ocornut/imgui) grapical user interface and its [ImNodes](https://github.com/rokups/ImNodes/blob/master/ImNodes.h) extension.
