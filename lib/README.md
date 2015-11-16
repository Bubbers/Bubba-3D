#FOR WINDOWS ONLY

These should be all the libraries you need to compile the project. <br />

Link the following in cmake:

#ASSIMP
ASSIMP_INCLUDE_DIR      -    Bubba-3D/lib <br />
ASSIMP_LIBRARY          -    Bubba-3D/lib/assimp.lib <br />
ASSIMP_LIBRARY_DEBUG    -    Bubba-3D/lib/assimp.lib <br />
ASSIMP_LIBRARY_RELEASE  -    Bubba-3D/lib/assimp.lib <br />

#GLEW
GLEW_INCLUDE_DIR        -    Bubba-3D/lib <br />
GLEW_LIBRARY            -    Bubba-3D/lib/glew32s.lib <br />

#GLUT
GLUT_INCLUDE_DIR        -    Bubba-3D/lib <br />
GLUT_glut_LIBRARY       -    Bubba-3D/lib/freeglutd.lib <br />

#FREEIMAGE              
FREEIMAGE_DIR           -    Bubba-3D/lib/freeimage <br />
FREEIMAGE_FOUND         -    Check <br />
FREEIMAGE_INCLUDE_PATH  -    Bubba-3D/lib/freeimage <br />
FREEIMAGE_LIBRARY       -    Bubba-3D/lib/freeimage/FreeImage.lib <br />
