#Bubba-3D!
This is a small 3D-engine programmed in C++
Originally implemented as a school project, now extended for fun!

###Status
[![Build Status](https://travis-ci.org/Bubbad/Bubba-3D.svg?branch=develop)](https://travis-ci.org/Bubbad/Bubba-3D)

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
* Particle system

###Current todos
* Refactor cube maps
* Finish octrees / implement AABB collision structure
* Support skinned meshes
* Optimizations!

***
###Example
Code example is outdated, new coming soon!.

###Installation
Requirements:
* <a href="http://www.cmake.org/">CMake</a>
* <a href="http://assimp.sourceforge.net/">Assimp</a>
* <a href="http://freeglut.sourceforge.net/">Glut (Freeglut)</a>
* <a href="http://glew.sourceforge.net/">Glew</a>
* <a href="http://freeimage.sourceforge.net/">FreeImage</a><br />

Linux: Install using the Makefile supplied in root map. run make install. This will install all dependencies for the project. Then you may run it with make run.

Windows: Either download the dependencies yourself from the links above, or use the specified libs in the libs folder. For more info how to config cmake using these, see README.md in libs/. 

Feel free to contact me regarding questions and/or installation trouble!
