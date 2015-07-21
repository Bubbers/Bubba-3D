#Bubba-3D!
This is a small game engine programmed in C++
using Glut and Glew.  Managed by cmake.
Originally implemented as a school project, now extended for fun!

***  
###Implemented
* Mesh loading using Assimp
* Normal maps
* Different lights (Directional, spot and point lights)
* Directional shadow map
* Fog
* Blur
* Skyboxes
* Dynamic / static cube maps
* Perspective camera

###Current todos
* Refactor cube maps
* Finish octrees / implement AABB collision structure
* Support skinned meshes
* lots more fun stuff!

***
###Example
Outdated, new coming soon!

###Installation
Requirements:
CMake            http://www.cmake.org/
Assimp           http://assimp.sourceforge.net/
Glut (Freeglut)  http://freeglut.sourceforge.net/
Glew             http://glew.sourceforge.net/
DevIL            http://openil.sourceforge.net/
For installation of these, please see their homepage.

Linux: Install using the Makefile supplied in root map. You might need to change the assimp path in CMakeLists.txt first.

Windows: Use CMake gui. Add the source catalogue and a build directory and then configure and generate! 