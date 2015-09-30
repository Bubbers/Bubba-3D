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
* Particle system
* Optimizations!

***
###Example
Code example is outdated, new coming soon!
Executable files are available for linux and win32 in the bin folder.

###Installation
Requirements:
* <a href="http://www.cmake.org/">CMake</a>
* <a href="http://assimp.sourceforge.net/">Assimp</a>
* <a href="http://freeglut.sourceforge.net/">Glut (Freeglut)</a>
* <a href="http://glew.sourceforge.net/">Glew</a>
* <a href="http://openil.sourceforge.net/">DevIL</a><br />
For installation of these, please see their homepage.

Linux: Install using the Makefile supplied in root map. You might need to change the assimp path in CMakeLists.txt first.

Windows: Use CMake gui. Add the source catalogue and a build directory and then configure and generate! 

Feel free to contact me regarding questions and/or installation trouble!
