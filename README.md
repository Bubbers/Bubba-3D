Bubba-3D [![Build Status](https://travis-ci.org/Bubbers/Bubba-3D.svg?branch=develop)](https://travis-ci.org/Bubbers/Bubba-3D)
========

About
-----
This is a small 3D Game-Engine programmed in C++ using openGL.
Originally implemented as a school project, now extended for fun!

Usage
-----
Tutorials can be found on the [wiki](https://github.com/Bubbers/Bubba-3D/wiki)

Implemented
-----------
* Mesh loading using Assimp
* Normal maps
* Different lights (Directional, spot and point lights)
* Directional shadow map
* Fog
* Blur
* Skyboxes
* Static cube maps
* Perspective camera
* Particle system
* Input and sound managers
* Collision

Installation
------------
### Requirements:
* <a href="http://www.cmake.org/">CMake</a>
* <a href="http://assimp.sourceforge.net/">Assimp</a>
* <a href="http://freeglut.sourceforge.net/">Glut (Freeglut)</a>
* <a href="http://glew.sourceforge.net/">Glew</a>
* <a href="http://freeimage.sourceforge.net/">FreeImage</a><br />
* <a href="http://www.sfml-dev.org/index.php">SFML</a><br />

### Install dependencies
Linux: Install using the Makefile supplied in root map. run make install. This will install all dependencies for the project. Then you may run it with make run.

Windows: Either download the dependencies yourself from the links above, or use the specified libs in the libs folder. For more info how to config cmake using these, see README.md in libs/. 

### Build Bubba-3D
```bash
mkdir build
cd build
cmake ..
make
```

